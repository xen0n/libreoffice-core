/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#include <memory>
#include <unistd.h>
#include <osl/thread.h>

#include <unx/fontmanager.hxx>
#include <fontsubset.hxx>
#include <impfontcharmap.hxx>
#include <unx/gendata.hxx>
#include <unx/helper.hxx>
#include <vcl/fontcharmap.hxx>

#include <tools/urlobj.hxx>

#include <osl/file.hxx>

#include <rtl/ustrbuf.hxx>
#include <rtl/strbuf.hxx>

#include <sal/macros.h>
#include <sal/log.hxx>

#include <i18nlangtag/applelangid.hxx>

#include <sft.hxx>

#if OSL_DEBUG_LEVEL > 1
#include <sys/times.h>
#include <stdio.h>
#endif

#include <algorithm>
#include <set>

#ifdef CALLGRIND_COMPILE
#include <valgrind/callgrind.h>
#endif

#include <com/sun/star/beans/XMaterialHolder.hpp>

using namespace vcl;
using namespace utl;
using namespace psp;
using namespace osl;
using namespace com::sun::star::uno;
using namespace com::sun::star::beans;
using namespace com::sun::star::lang;

/*
 *  static helpers
 */

static sal_uInt16 getUInt16BE( const sal_uInt8*& pBuffer )
{
    sal_uInt16 nRet = static_cast<sal_uInt16>(pBuffer[1]) |
        (static_cast<sal_uInt16>(pBuffer[0]) << 8);
    pBuffer+=2;
    return nRet;
}

/*
 *  PrintFont implementations
 */
PrintFontManager::PrintFont::PrintFont()
:   m_eFamilyStyle(FAMILY_DONTKNOW)
,   m_eItalic(ITALIC_DONTKNOW)
,   m_eWidth(WIDTH_DONTKNOW)
,   m_eWeight(WEIGHT_DONTKNOW)
,   m_ePitch(PITCH_DONTKNOW)
,   m_aEncoding(RTL_TEXTENCODING_DONTKNOW)
,   m_nAscend(0)
,   m_nDescend(0)
,   m_nLeading(0)
,   m_nXMin(0)
,   m_nYMin(0)
,   m_nXMax(0)
,   m_nYMax(0)
,   m_nDirectory(0)
,   m_nCollectionEntry(0)
,   m_nVariationEntry(0)
{
}

/*
 *  one instance only
 */
PrintFontManager& PrintFontManager::get()
{
    GenericUnixSalData* const pSalData(GetGenericUnixSalData());
    assert(pSalData);
    return *pSalData->GetPrintFontManager();
}

/*
 *  the PrintFontManager
 */

PrintFontManager::PrintFontManager()
    : m_nNextFontID( 1 )
    , m_nNextDirAtom( 1 )
    , m_aFontInstallerTimer("PrintFontManager m_aFontInstallerTimer")
{
    m_aFontInstallerTimer.SetInvokeHandler(LINK(this, PrintFontManager, autoInstallFontLangSupport));
    m_aFontInstallerTimer.SetTimeout(5000);
}

PrintFontManager::~PrintFontManager()
{
    m_aFontInstallerTimer.Stop();
    deinitFontconfig();
}

OString PrintFontManager::getDirectory( int nAtom ) const
{
    std::unordered_map< int, OString >::const_iterator it( m_aAtomToDir.find( nAtom ) );
    return it != m_aAtomToDir.end() ? it->second : OString();
}

int PrintFontManager::getDirectoryAtom( const OString& rDirectory )
{
    int nAtom = 0;
    std::unordered_map< OString, int >::const_iterator it
          ( m_aDirToAtom.find( rDirectory ) );
    if( it != m_aDirToAtom.end() )
        nAtom = it->second;
    else
    {
        nAtom = m_nNextDirAtom++;
        m_aDirToAtom[ rDirectory ] = nAtom;
        m_aAtomToDir[ nAtom ] = rDirectory;
    }
    return nAtom;
}

std::vector<fontID> PrintFontManager::addFontFile( std::u16string_view rFileUrl )
{
    rtl_TextEncoding aEncoding = osl_getThreadTextEncoding();
    INetURLObject aPath( rFileUrl );
    OString aName(OUStringToOString(aPath.GetLastName(INetURLObject::DecodeMechanism::WithCharset, aEncoding), aEncoding));
    OString aDir( OUStringToOString(
        INetURLObject::decode( aPath.GetPath(), INetURLObject::DecodeMechanism::WithCharset, aEncoding ), aEncoding ) );

    int nDirID = getDirectoryAtom( aDir );
    std::vector<fontID> aFontIds = findFontFileIDs( nDirID, aName );
    if( aFontIds.empty() )
    {
        std::vector<PrintFont> aNewFonts = analyzeFontFile(nDirID, aName);
        for (auto & font : aNewFonts)
        {
            fontID nFontId = m_nNextFontID++;
            m_aFonts[nFontId] = std::move(font);
            m_aFontFileToFontID[ aName ].insert( nFontId );
            aFontIds.push_back(nFontId);
        }
    }
    return aFontIds;
}

std::vector<PrintFontManager::PrintFont> PrintFontManager::analyzeFontFile( int nDirID, const OString& rFontFile, const char *pFormat ) const
{
    std::vector<PrintFontManager::PrintFont> aNewFonts;

    OString aDir( getDirectory( nDirID ) );

    OString aFullPath = aDir + "/" + rFontFile;

    // #i1872# reject unreadable files
    if( access( aFullPath.getStr(), R_OK ) )
        return aNewFonts;

    bool bSupported = false;
    if (pFormat)
    {
        if (!strcmp(pFormat, "TrueType") ||
            !strcmp(pFormat, "CFF"))
            bSupported = true;
    }
    if (!bSupported)
    {
        OString aExt( rFontFile.copy( rFontFile.lastIndexOf( '.' )+1 ) );
        if( aExt.equalsIgnoreAsciiCase("ttf")
             ||  aExt.equalsIgnoreAsciiCase("ttc")
             ||  aExt.equalsIgnoreAsciiCase("tte")   // #i33947# for Gaiji support
             ||  aExt.equalsIgnoreAsciiCase("otf") ) // check for TTF- and PS-OpenType too
            bSupported = true;
    }

    if (bSupported)
    {
        // get number of ttc entries
        int nLength = CountTTCFonts( aFullPath.getStr() );
        if (nLength > 0)
        {
            SAL_INFO("vcl.fonts", "ttc: " << aFullPath << " contains " << nLength << " fonts");

            sal_uInt64 fileSize = 0;

            OUString aURL;
            if (osl::File::getFileURLFromSystemPath(OStringToOUString(aFullPath, osl_getThreadTextEncoding()),
                aURL) == osl::File::E_None)
            {
                osl::File aFile(aURL);
                if (aFile.open(osl_File_OpenFlag_Read | osl_File_OpenFlag_NoLock) == osl::File::E_None)
                {
                    osl::DirectoryItem aItem;
                    if (osl::DirectoryItem::get(aURL, aItem) == osl::File::E_None)
                    {
                        osl::FileStatus aFileStatus( osl_FileStatus_Mask_FileSize );
                        if (aItem.getFileStatus(aFileStatus) == osl::File::E_None)
                            fileSize = aFileStatus.getFileSize();
                    }
                }
            }

            //Feel free to calc the exact max possible number of fonts a file
            //could contain given its physical size. But this will clamp it to
            //a sane starting point
            //http://processingjs.nihongoresources.com/the_smallest_font/
            //https://github.com/grzegorzrolek/null-ttf
            const int nMaxFontsPossible = fileSize / 528;
            if (nLength > nMaxFontsPossible)
                nLength = nMaxFontsPossible;

            for( int i = 0; i < nLength; i++ )
            {
                PrintFont aFont;
                aFont.m_nDirectory         = nDirID;
                aFont.m_aFontFile          = rFontFile;
                aFont.m_nCollectionEntry   = i;
                if (analyzeSfntFile(aFont))
                    aNewFonts.push_back(aFont);
            }
        }
        else
        {
            PrintFont aFont;
            aFont.m_nDirectory         = nDirID;
            aFont.m_aFontFile          = rFontFile;
            aFont.m_nCollectionEntry   = 0;

            // need to read the font anyway to get aliases inside the font file
            if (analyzeSfntFile(aFont))
                aNewFonts.push_back(aFont);
        }
    }
    return aNewFonts;
}

fontID PrintFontManager::findFontFileID(int nDirID, const OString& rFontFile, int nFaceIndex, int nVariationIndex) const
{
    fontID nID = 0;

    std::unordered_map< OString, ::std::set< fontID > >::const_iterator set_it = m_aFontFileToFontID.find( rFontFile );
    if( set_it == m_aFontFileToFontID.end() )
        return nID;

    for (auto const& elem : set_it->second)
    {
        auto it = m_aFonts.find(elem);
        if( it == m_aFonts.end() )
            continue;
        const PrintFont& rFont = (*it).second;
        if (rFont.m_nDirectory == nDirID &&
            rFont.m_aFontFile == rFontFile &&
            rFont.m_nCollectionEntry == nFaceIndex &&
            rFont.m_nVariationEntry == nVariationIndex)
        {
            nID = it->first;
            if (nID)
                break;
        }
    }

    return nID;
}

std::vector<fontID> PrintFontManager::findFontFileIDs( int nDirID, const OString& rFontFile ) const
{
    std::vector<fontID> aIds;

    std::unordered_map< OString, ::std::set< fontID > >::const_iterator set_it = m_aFontFileToFontID.find( rFontFile );
    if( set_it == m_aFontFileToFontID.end() )
        return aIds;

    for (auto const& elem : set_it->second)
    {
        auto it = m_aFonts.find(elem);
        if( it == m_aFonts.end() )
            continue;
        const PrintFont& rFont = (*it).second;
        if (rFont.m_nDirectory == nDirID &&
            rFont.m_aFontFile == rFontFile)
            aIds.push_back(it->first);
    }

    return aIds;
}

OUString PrintFontManager::convertSfntName( void* pRecord )
{
    NameRecord* pNameRecord = static_cast<NameRecord*>(pRecord);
    OUString aValue;
    if(
       ( pNameRecord->platformID == 3 && ( pNameRecord->encodingID == 0 || pNameRecord->encodingID == 1 ) )  // MS, Unicode
       ||
       ( pNameRecord->platformID == 0 ) // Apple, Unicode
       )
    {
        OUStringBuffer aName( pNameRecord->slen/2 );
        const sal_uInt8* pNameBuffer = pNameRecord->sptr;
        for(int n = 0; n < pNameRecord->slen/2; n++ )
            aName.append( static_cast<sal_Unicode>(getUInt16BE( pNameBuffer )) );
        aValue = aName.makeStringAndClear();
    }
    else if( pNameRecord->platformID == 3 )
    {
        if( pNameRecord->encodingID >= 2 && pNameRecord->encodingID <= 6 )
        {
            /*
             *  and now for a special kind of madness:
             *  some fonts encode their byte value string as BE uint16
             *  (leading to stray zero bytes in the string)
             *  while others code two bytes as a uint16 and swap to BE
             */
            OStringBuffer aName;
            const sal_uInt8* pNameBuffer = pNameRecord->sptr;
            for(int n = 0; n < pNameRecord->slen/2; n++ )
            {
                sal_Unicode aCode = static_cast<sal_Unicode>(getUInt16BE( pNameBuffer ));
                char aChar = aCode >> 8;
                if( aChar )
                    aName.append( aChar );
                aChar = aCode & 0x00ff;
                if( aChar )
                    aName.append( aChar );
            }
            switch( pNameRecord->encodingID )
            {
                case 2:
                    aValue = OStringToOUString( aName, RTL_TEXTENCODING_MS_932 );
                    break;
                case 3:
                    aValue = OStringToOUString( aName, RTL_TEXTENCODING_MS_936 );
                    break;
                case 4:
                    aValue = OStringToOUString( aName, RTL_TEXTENCODING_MS_950 );
                    break;
                case 5:
                    aValue = OStringToOUString( aName, RTL_TEXTENCODING_MS_949 );
                    break;
                case 6:
                    aValue = OStringToOUString( aName, RTL_TEXTENCODING_MS_1361 );
                    break;
            }
        }
    }
    else if( pNameRecord->platformID == 1 )
    {
        OString aName(reinterpret_cast<char*>(pNameRecord->sptr), pNameRecord->slen);
        rtl_TextEncoding eEncoding = RTL_TEXTENCODING_DONTKNOW;
        switch (pNameRecord->encodingID)
        {
            case 0:
                eEncoding = RTL_TEXTENCODING_APPLE_ROMAN;
                break;
            case 1:
                eEncoding = RTL_TEXTENCODING_APPLE_JAPANESE;
                break;
            case 2:
                eEncoding = RTL_TEXTENCODING_APPLE_CHINTRAD;
                break;
            case 3:
                eEncoding = RTL_TEXTENCODING_APPLE_KOREAN;
                break;
            case 4:
                eEncoding = RTL_TEXTENCODING_APPLE_ARABIC;
                break;
            case 5:
                eEncoding = RTL_TEXTENCODING_APPLE_HEBREW;
                break;
            case 6:
                eEncoding = RTL_TEXTENCODING_APPLE_GREEK;
                break;
            case 7:
                eEncoding = RTL_TEXTENCODING_APPLE_CYRILLIC;
                break;
            case 9:
                eEncoding = RTL_TEXTENCODING_APPLE_DEVANAGARI;
                break;
            case 10:
                eEncoding = RTL_TEXTENCODING_APPLE_GURMUKHI;
                break;
            case 11:
                eEncoding = RTL_TEXTENCODING_APPLE_GUJARATI;
                break;
            case 21:
                eEncoding = RTL_TEXTENCODING_APPLE_THAI;
                break;
            case 25:
                eEncoding = RTL_TEXTENCODING_APPLE_CHINSIMP;
                break;
            case 29:
                eEncoding = RTL_TEXTENCODING_APPLE_CENTEURO;
                break;
            case 32:    //Uninterpreted
                eEncoding = RTL_TEXTENCODING_UTF8;
                break;
            default:
                if (aName.startsWith("Khmer OS"))
                    eEncoding = RTL_TEXTENCODING_UTF8;
                SAL_WARN_IF(eEncoding == RTL_TEXTENCODING_DONTKNOW, "vcl.fonts", "Unimplemented mac encoding " << pNameRecord->encodingID << " to unicode conversion for fontname " << aName);
                break;
        }
        if (eEncoding != RTL_TEXTENCODING_DONTKNOW)
            aValue = OStringToOUString(aName, eEncoding);
    }

    return aValue;
}

//fdo#33349.There exists an archaic Berling Antiqua font which has a "Times New
//Roman" name field in it. We don't want the "Times New Roman" name to take
//precedence in this case. We take Berling Antiqua as a higher priority name,
//and erase the "Times New Roman" name
namespace
{
    bool isBadTNR(std::u16string_view rName, ::std::set< OUString >& rSet)
    {
        bool bRet = false;
        if ( rName == u"Berling Antiqua" )
        {
            ::std::set< OUString >::iterator aEnd = rSet.end();
            ::std::set< OUString >::iterator aI = rSet.find("Times New Roman");
            if (aI != aEnd)
            {
                bRet = true;
                rSet.erase(aI);
            }
        }
        return bRet;
    }
}

void PrintFontManager::analyzeSfntFamilyName( void const * pTTFont, ::std::vector< OUString >& rNames )
{
    OUString aFamily;

    rNames.clear();
    ::std::set< OUString > aSet;

    NameRecord* pNameRecords = nullptr;
    int nNameRecords = GetTTNameRecords( static_cast<TrueTypeFont const *>(pTTFont), &pNameRecords );
    if( nNameRecords && pNameRecords )
    {
        LanguageTag aSystem("");
        LanguageType eLang = aSystem.getLanguageType();
        int nLastMatch = -1;
        for( int i = 0; i < nNameRecords; i++ )
        {
            if( pNameRecords[i].nameID != 1 || pNameRecords[i].sptr == nullptr )
                continue;
            int nMatch = -1;
            if( pNameRecords[i].platformID == 0 ) // Unicode
                nMatch = 4000;
            else if( pNameRecords[i].platformID == 3 )
            {
                // this bases on the LanguageType actually being a Win LCID
                if (pNameRecords[i].languageID == eLang)
                    nMatch = 8000;
                else if( pNameRecords[i].languageID == LANGUAGE_ENGLISH_US )
                    nMatch = 2000;
                else if( pNameRecords[i].languageID == LANGUAGE_ENGLISH ||
                         pNameRecords[i].languageID == LANGUAGE_ENGLISH_UK )
                    nMatch = 1500;
                else
                    nMatch = 1000;
            }
            else if (pNameRecords[i].platformID == 1)
            {
                AppleLanguageId aAppleId = static_cast<AppleLanguageId>(static_cast<sal_uInt16>(pNameRecords[i].languageID));
                LanguageTag aApple(makeLanguageTagFromAppleLanguageId(aAppleId));
                if (aApple == aSystem)
                    nMatch = 8000;
                else if (aAppleId == AppleLanguageId::ENGLISH)
                    nMatch = 2000;
                else
                    nMatch = 1000;
            }
            OUString aName = convertSfntName( pNameRecords + i );
            aSet.insert( aName );
            if (aName.isEmpty())
                continue;
            if( nMatch > nLastMatch || isBadTNR(aName, aSet) )
            {
                nLastMatch = nMatch;
                aFamily = aName;
            }
        }
    }
    DisposeNameRecords( pNameRecords, nNameRecords );
    if( !aFamily.isEmpty() )
    {
        rNames.push_back( aFamily );
        for (auto const& elem : aSet)
            if( elem != aFamily )
                rNames.push_back(elem);
    }
}

bool PrintFontManager::analyzeSfntFile( PrintFont& rFont ) const
{
    bool bSuccess = false;
    rtl_TextEncoding aEncoding = osl_getThreadTextEncoding();
    OString aFile = getFontFile( rFont );
    TrueTypeFont* pTTFont = nullptr;

    auto const e = OpenTTFontFile( aFile.getStr(), rFont.m_nCollectionEntry, &pTTFont );
    if( e == SFErrCodes::Ok )
    {
        TTGlobalFontInfo aInfo;
        GetTTGlobalFontInfo( pTTFont, & aInfo );

        ::std::vector< OUString > aNames;
        analyzeSfntFamilyName( pTTFont, aNames );

        // set family name from XLFD if possible
        if (rFont.m_aFamilyName.isEmpty())
        {
            if( !aNames.empty() )
            {
                rFont.m_aFamilyName = aNames.front();
                aNames.erase(aNames.begin());
            }
            else
            {
                 sal_Int32   dotIndex;

                 // poor font does not have a family name
                 // name it to file name minus the extension
                 dotIndex = rFont.m_aFontFile.lastIndexOf( '.' );
                 if ( dotIndex == -1 )
                     dotIndex = rFont.m_aFontFile.getLength();

                 rFont.m_aFamilyName = OStringToOUString(rFont.m_aFontFile.subView(0, dotIndex), aEncoding);
            }
        }
        for (auto const& aAlias : aNames)
        {
            if (!aAlias.isEmpty())
            {
                if (rFont.m_aFamilyName != aAlias)
                {
                    auto al_it = std::find(rFont.m_aAliases.begin(), rFont.m_aAliases.end(), aAlias);
                    if( al_it == rFont.m_aAliases.end() )
                        rFont.m_aAliases.push_back(aAlias);
                }
            }
        }

        if( aInfo.usubfamily )
            rFont.m_aStyleName = OUString( aInfo.usubfamily );

        SAL_WARN_IF( !aInfo.psname, "vcl.fonts", "No PostScript name in font:" << aFile );

        rFont.m_aPSName = aInfo.psname ?
            OUString(aInfo.psname, rtl_str_getLength(aInfo.psname), aEncoding) :
            rFont.m_aFamilyName; // poor font does not have a postscript name

        rFont.m_eFamilyStyle = matchFamilyName(rFont.m_aFamilyName);

        switch( aInfo.weight )
        {
            case FW_THIN:           rFont.m_eWeight = WEIGHT_THIN; break;
            case FW_EXTRALIGHT: rFont.m_eWeight = WEIGHT_ULTRALIGHT; break;
            case FW_LIGHT:          rFont.m_eWeight = WEIGHT_LIGHT; break;
            case FW_MEDIUM:     rFont.m_eWeight = WEIGHT_MEDIUM; break;
            case FW_SEMIBOLD:       rFont.m_eWeight = WEIGHT_SEMIBOLD; break;
            case FW_BOLD:           rFont.m_eWeight = WEIGHT_BOLD; break;
            case FW_EXTRABOLD:      rFont.m_eWeight = WEIGHT_ULTRABOLD; break;
            case FW_BLACK:          rFont.m_eWeight = WEIGHT_BLACK; break;

            case FW_NORMAL:
            default:        rFont.m_eWeight = WEIGHT_NORMAL; break;
        }

        switch( aInfo.width )
        {
            case FWIDTH_ULTRA_CONDENSED:    rFont.m_eWidth = WIDTH_ULTRA_CONDENSED; break;
            case FWIDTH_EXTRA_CONDENSED:    rFont.m_eWidth = WIDTH_EXTRA_CONDENSED; break;
            case FWIDTH_CONDENSED:          rFont.m_eWidth = WIDTH_CONDENSED; break;
            case FWIDTH_SEMI_CONDENSED: rFont.m_eWidth = WIDTH_SEMI_CONDENSED; break;
            case FWIDTH_SEMI_EXPANDED:      rFont.m_eWidth = WIDTH_SEMI_EXPANDED; break;
            case FWIDTH_EXPANDED:           rFont.m_eWidth = WIDTH_EXPANDED; break;
            case FWIDTH_EXTRA_EXPANDED: rFont.m_eWidth = WIDTH_EXTRA_EXPANDED; break;
            case FWIDTH_ULTRA_EXPANDED: rFont.m_eWidth = WIDTH_ULTRA_EXPANDED; break;

            case FWIDTH_NORMAL:
            default:                        rFont.m_eWidth = WIDTH_NORMAL; break;
        }

        rFont.m_ePitch = aInfo.pitch ? PITCH_FIXED : PITCH_VARIABLE;
        rFont.m_eItalic = aInfo.italicAngle == 0 ? ITALIC_NONE : ( aInfo.italicAngle < 0 ? ITALIC_NORMAL : ITALIC_OBLIQUE );
        // #104264# there are fonts that set italic angle 0 although they are
        // italic; use macstyle bit here
        if( aInfo.italicAngle == 0 && (aInfo.macStyle & 2) )
            rFont.m_eItalic = ITALIC_NORMAL;

        rFont.m_aEncoding = aInfo.symbolEncoded ? RTL_TEXTENCODING_SYMBOL : RTL_TEXTENCODING_UCS2;

        if( aInfo.ascender && aInfo.descender )
        {
            rFont.m_nLeading   = aInfo.linegap;
            rFont.m_nAscend    = aInfo.ascender;
            rFont.m_nDescend   = -aInfo.descender;
        }
        else if( aInfo.typoAscender && aInfo.typoDescender )
        {
            rFont.m_nLeading   = aInfo.typoLineGap;
            rFont.m_nAscend    = aInfo.typoAscender;
            rFont.m_nDescend   = -aInfo.typoDescender;
        }
        else if( aInfo.winAscent && aInfo.winDescent )
        {
            rFont.m_nAscend    = aInfo.winAscent;
            rFont.m_nDescend   = aInfo.winDescent;
            rFont.m_nLeading   = rFont.m_nAscend + rFont.m_nDescend - 1000;
        }

        // last try: font bounding box
        if( rFont.m_nAscend == 0 )
            rFont.m_nAscend = aInfo.yMax;
        if( rFont.m_nDescend == 0 )
            rFont.m_nDescend = -aInfo.yMin;
        if( rFont.m_nLeading == 0 )
            rFont.m_nLeading = 15 * (rFont.m_nAscend+rFont.m_nDescend) / 100;

        // get bounding box
        rFont.m_nXMin = aInfo.xMin;
        rFont.m_nYMin = aInfo.yMin;
        rFont.m_nXMax = aInfo.xMax;
        rFont.m_nYMax = aInfo.yMax;

        CloseTTFont( pTTFont );
        bSuccess = true;
    }
    else
        SAL_WARN("vcl.fonts", "Could not OpenTTFont \"" << aFile << "\": " << int(e));

    return bSuccess;
}

void PrintFontManager::initialize()
{
    #ifdef CALLGRIND_COMPILE
    CALLGRIND_TOGGLE_COLLECT();
    CALLGRIND_ZERO_STATS();
    #endif

    // initialize can be called more than once, e.g.
    // gtk-fontconfig-timestamp changes to reflect new font installed and
    // PrintFontManager::initialize called again
    {
        m_nNextFontID = 1;
        m_aFonts.clear();
    }
#if OSL_DEBUG_LEVEL > 1
    clock_t aStart;
    clock_t aStep1;
    clock_t aStep2;

    struct tms tms;

    aStart = times( &tms );
#endif

    // first try fontconfig
    initFontconfig();

    // part one - look for downloadable fonts
    rtl_TextEncoding aEncoding = osl_getThreadTextEncoding();
    const OUString &rSalPrivatePath = psp::getFontPath();

    // search for the fonts in SAL_PRIVATE_FONTPATH first; those are
    // the fonts installed with the office
    if( !rSalPrivatePath.isEmpty() )
    {
        OString aPath = OUStringToOString( rSalPrivatePath, aEncoding );
        sal_Int32 nIndex = 0;
        do
        {
            OString aToken = aPath.getToken( 0, ';', nIndex );
            normPath( aToken );
            if (!aToken.isEmpty())
                addFontconfigDir(aToken);
        } while( nIndex >= 0 );
    }

    countFontconfigFonts();

#if OSL_DEBUG_LEVEL > 1
    aStep1 = times( &tms );

    aStep2 = times( &tms );
    SAL_INFO("vcl.fonts", "PrintFontManager::initialize: collected "
            << m_aFonts.size()
            << " fonts.");
    double fTick = (double)sysconf( _SC_CLK_TCK );
    SAL_INFO("vcl.fonts", "Step 1 took "
            << ((double)(aStep1 - aStart)/fTick)
            << " seconds.");
    SAL_INFO("vcl.fonts", "Step 2 took "
            << ((double)(aStep2 - aStep1)/fTick)
            << " seconds.");
#endif

    #ifdef CALLGRIND_COMPILE
    CALLGRIND_DUMP_STATS();
    CALLGRIND_TOGGLE_COLLECT();
    #endif
}

void PrintFontManager::getFontList( ::std::vector< fontID >& rFontIDs )
{
    rFontIDs.clear();

    for (auto const& font : m_aFonts)
        rFontIDs.push_back(font.first);
}

void PrintFontManager::fillPrintFontInfo(const PrintFont& rFont, FastPrintFontInfo& rInfo)
{
    rInfo.m_aFamilyName     = rFont.m_aFamilyName;
    rInfo.m_aStyleName      = rFont.m_aStyleName;
    rInfo.m_eFamilyStyle    = rFont.m_eFamilyStyle;
    rInfo.m_eItalic         = rFont.m_eItalic;
    rInfo.m_eWidth          = rFont.m_eWidth;
    rInfo.m_eWeight         = rFont.m_eWeight;
    rInfo.m_ePitch          = rFont.m_ePitch;
    rInfo.m_aEncoding       = rFont.m_aEncoding;
    rInfo.m_aAliases        = rFont.m_aAliases;
}

void PrintFontManager::fillPrintFontInfo( PrintFont& rFont, PrintFontInfo& rInfo ) const
{
    if (rFont.m_nAscend == 0 && rFont.m_nDescend == 0)
    {
        analyzeSfntFile(rFont);
    }

    fillPrintFontInfo( rFont, static_cast< FastPrintFontInfo& >( rInfo ) );

    rInfo.m_nAscend         = rFont.m_nAscend;
    rInfo.m_nDescend        = rFont.m_nDescend;
}

bool PrintFontManager::getFontInfo( fontID nFontID, PrintFontInfo& rInfo ) const
{
    const PrintFont* pFont = getFont( nFontID );
    if( pFont )
    {
        rInfo.m_nID = nFontID;
        fillPrintFontInfo( *pFont, rInfo );
    }
    return pFont != nullptr;
}

bool PrintFontManager::getFontFastInfo( fontID nFontID, FastPrintFontInfo& rInfo ) const
{
    const PrintFont* pFont = getFont( nFontID );
    if( pFont )
    {
        rInfo.m_nID = nFontID;
        fillPrintFontInfo( *pFont, rInfo );
    }
    return pFont != nullptr;
}

void PrintFontManager::getFontBoundingBox( fontID nFontID, int& xMin, int& yMin, int& xMax, int& yMax )
{
    PrintFont* pFont = getFont( nFontID );
    if( pFont )
    {
        if( pFont->m_nXMin == 0 && pFont->m_nYMin == 0 && pFont->m_nXMax == 0 && pFont->m_nYMax == 0 )
        {
            analyzeSfntFile(*pFont);
        }
        xMin = pFont->m_nXMin;
        yMin = pFont->m_nYMin;
        xMax = pFont->m_nXMax;
        yMax = pFont->m_nYMax;
    }
}

int PrintFontManager::getFontFaceNumber( fontID nFontID ) const
{
    int nRet = 0;
    const PrintFont* pFont = getFont( nFontID );
    if (pFont)
    {
        nRet = pFont->m_nCollectionEntry;
        if (nRet < 0)
            nRet = 0;
    }
    return nRet;
}

int PrintFontManager::getFontFaceVariation( fontID nFontID ) const
{
    int nRet = 0;
    const PrintFont* pFont = getFont( nFontID );
    if (pFont)
    {
        nRet = pFont->m_nVariationEntry;
        if (nRet < 0)
            nRet = 0;
    }
    return nRet;
}

FontFamily PrintFontManager::matchFamilyName( std::u16string_view rFamily )
{
    struct family_t {
        const char*  mpName;
        sal_uInt16   mnLength;
        FontFamily   meType;
    };

#define InitializeClass( p, a ) p, sizeof(p) - 1, a
    static const family_t pFamilyMatch[] =  {
        { InitializeClass( "arial",                  FAMILY_SWISS )  },
        { InitializeClass( "arioso",                 FAMILY_SCRIPT ) },
        { InitializeClass( "avant garde",            FAMILY_SWISS )  },
        { InitializeClass( "avantgarde",             FAMILY_SWISS )  },
        { InitializeClass( "bembo",                  FAMILY_ROMAN )  },
        { InitializeClass( "bookman",                FAMILY_ROMAN )  },
        { InitializeClass( "conga",                  FAMILY_ROMAN )  },
        { InitializeClass( "courier",                FAMILY_MODERN ) },
        { InitializeClass( "curl",                   FAMILY_SCRIPT ) },
        { InitializeClass( "fixed",                  FAMILY_MODERN ) },
        { InitializeClass( "gill",                   FAMILY_SWISS )  },
        { InitializeClass( "helmet",                 FAMILY_MODERN ) },
        { InitializeClass( "helvetica",              FAMILY_SWISS )  },
        { InitializeClass( "international",          FAMILY_MODERN ) },
        { InitializeClass( "lucida",                 FAMILY_SWISS )  },
        { InitializeClass( "new century schoolbook", FAMILY_ROMAN )  },
        { InitializeClass( "palatino",               FAMILY_ROMAN )  },
        { InitializeClass( "roman",                  FAMILY_ROMAN )  },
        { InitializeClass( "sans serif",             FAMILY_SWISS )  },
        { InitializeClass( "sansserif",              FAMILY_SWISS )  },
        { InitializeClass( "serf",                   FAMILY_ROMAN )  },
        { InitializeClass( "serif",                  FAMILY_ROMAN )  },
        { InitializeClass( "times",                  FAMILY_ROMAN )  },
        { InitializeClass( "utopia",                 FAMILY_ROMAN )  },
        { InitializeClass( "zapf chancery",          FAMILY_SCRIPT ) },
        { InitializeClass( "zapfchancery",           FAMILY_SCRIPT ) }
    };

    OString aFamily = OUStringToOString( rFamily, RTL_TEXTENCODING_ASCII_US );
    sal_uInt32 nLower = 0;
    sal_uInt32 nUpper = SAL_N_ELEMENTS(pFamilyMatch);

    while( nLower < nUpper )
    {
        sal_uInt32 nCurrent = (nLower + nUpper) / 2;
        const family_t* pHaystack = pFamilyMatch + nCurrent;
        sal_Int32  nComparison =
            rtl_str_compareIgnoreAsciiCase_WithLength
            (
             aFamily.getStr(), aFamily.getLength(),
             pHaystack->mpName, pHaystack->mnLength
             );

        if( nComparison < 0 )
            nUpper = nCurrent;
        else
            if( nComparison > 0 )
                nLower = nCurrent + 1;
            else
                return pHaystack->meType;
    }

    return FAMILY_DONTKNOW;
}

OString PrintFontManager::getFontFile(const PrintFont& rFont) const
{
    std::unordered_map< int, OString >::const_iterator it = m_aAtomToDir.find(rFont.m_nDirectory);
    OString aPath = it->second + "/" + rFont.m_aFontFile;
    return aPath;
}

OUString PrintFontManager::getPSName( fontID nFontID )
{
    PrintFont* pFont = getFont( nFontID );
    if (pFont && pFont->m_aPSName.isEmpty())
    {
        analyzeSfntFile(*pFont);
    }

    return pFont ? pFont->m_aPSName : OUString();
}

int PrintFontManager::getFontAscend( fontID nFontID )
{
    PrintFont* pFont = getFont( nFontID );
    if (pFont && pFont->m_nAscend == 0 && pFont->m_nDescend == 0)
    {
        analyzeSfntFile(*pFont);
    }
    return pFont ? pFont->m_nAscend : 0;
}

int PrintFontManager::getFontDescend( fontID nFontID )
{
    PrintFont* pFont = getFont( nFontID );
    if (pFont && pFont->m_nAscend == 0 && pFont->m_nDescend == 0)
    {
        analyzeSfntFile(*pFont);
    }
    return pFont ? pFont->m_nDescend : 0;
}

// TODO: move most of this stuff into the central font-subsetting code
bool PrintFontManager::createFontSubset(
                                        FontSubsetInfo& rInfo,
                                        fontID nFont,
                                        const OUString& rOutFile,
                                        const sal_GlyphId* pGlyphIds,
                                        const sal_uInt8* pNewEncoding,
                                        sal_Int32* pWidths,
                                        int nGlyphs
                                        )
{
    PrintFont* pFont = getFont( nFont );
    if( !pFont )
        return false;

    rInfo.m_nFontType = FontType::SFNT_TTF;

    // reshuffle array of requested glyphs to make sure glyph0==notdef
    sal_uInt8  pEnc[256];
    sal_uInt16 pGID[256];
    sal_uInt8  pOldIndex[256];
    memset( pEnc, 0, sizeof( pEnc ) );
    memset( pGID, 0, sizeof( pGID ) );
    memset( pOldIndex, 0, sizeof( pOldIndex ) );
    if( nGlyphs > 256 )
        return false;
    int nChar = 1;
    for( int i = 0; i < nGlyphs; i++ )
    {
        if( pNewEncoding[i] == 0 )
        {
            pOldIndex[ 0 ] = i;
        }
        else
        {
            SAL_WARN_IF( (pGlyphIds[i] & 0x007f0000), "vcl.fonts", "overlong glyph id" );
            SAL_WARN_IF( static_cast<int>(pNewEncoding[i]) >= nGlyphs, "vcl.fonts", "encoding wrong" );
            SAL_WARN_IF( pEnc[pNewEncoding[i]] != 0 || pGID[pNewEncoding[i]] != 0, "vcl.fonts", "duplicate encoded glyph" );
            pEnc[ pNewEncoding[i] ] = pNewEncoding[i];
            pGID[ pNewEncoding[i] ] = static_cast<sal_uInt16>(pGlyphIds[ i ]);
            pOldIndex[ pNewEncoding[i] ] = i;
            nChar++;
        }
    }
    nGlyphs = nChar; // either input value or increased by one

    // prepare system name for read access for subset source file
    // TODO: since this file is usually already mmapped there is no need to open it again
    const OString aFromFile = getFontFile( *pFont );

    TrueTypeFont* pTTFont = nullptr; // TODO: rename to SfntFont
    if( OpenTTFontFile( aFromFile.getStr(), pFont->m_nCollectionEntry, &pTTFont ) != SFErrCodes::Ok )
        return false;

    // prepare system name for write access for subset file target
    OUString aSysPath;
    if( osl_File_E_None != osl_getSystemPathFromFileURL( rOutFile.pData, &aSysPath.pData ) )
        return false;
    const rtl_TextEncoding aEncoding = osl_getThreadTextEncoding();
    const OString aToFile( OUStringToOString( aSysPath, aEncoding ) );

    // do CFF subsetting if possible
    sal_uInt32 nCffLength = 0;
    const sal_uInt8* pCffBytes = pTTFont->table(vcl::O_CFF, nCffLength);
    if (pCffBytes)
    {
        rInfo.LoadFont( FontType::CFF_FONT, pCffBytes, nCffLength );
#if 1 // TODO: remove 16bit->long conversion when related methods handle non-16bit glyphids
        sal_GlyphId aRequestedGlyphIds[256];
        for( int i = 0; i < nGlyphs; ++i )
            aRequestedGlyphIds[i] = pGID[i];
#endif
        // create subset file at requested path
        FILE* pOutFile = fopen( aToFile.getStr(), "wb" );
        if (!pOutFile)
        {
            CloseTTFont( pTTFont );
            return false;
        }
        // create font subset
        const char* const pGlyphSetName = nullptr; // TODO: better name?
        const bool bOK = rInfo.CreateFontSubset(
            FontType::TYPE1_PFB,
            pOutFile, pGlyphSetName,
            aRequestedGlyphIds, pEnc, nGlyphs, pWidths );
        fclose( pOutFile );
        // For OTC, values from hhea or OS2 are better
        psp::PrintFontInfo aFontInfo;
        if( getFontInfo( nFont, aFontInfo ) )
        {
            rInfo.m_nAscent     = aFontInfo.m_nAscend;
            rInfo.m_nDescent    = -aFontInfo.m_nDescend;
        }
        // cleanup before early return
        CloseTTFont( pTTFont );
        return bOK;
    }

    // do TTF->Type42 or Type3 subsetting
    // fill in font info
    psp::PrintFontInfo aFontInfo;
    if( ! getFontInfo( nFont, aFontInfo ) )
        return false;

    rInfo.m_nAscent     = aFontInfo.m_nAscend;
    rInfo.m_nDescent    = aFontInfo.m_nDescend;
    rInfo.m_aPSName     = getPSName( nFont );

    int xMin, yMin, xMax, yMax;
    getFontBoundingBox( nFont, xMin, yMin, xMax, yMax );
    rInfo.m_aFontBBox   = tools::Rectangle( Point( xMin, yMin ), Size( xMax-xMin, yMax-yMin ) );
    rInfo.m_nCapHeight  = yMax; // Well ...

    if (pWidths)
    {
        // fill in glyph advance widths
        std::unique_ptr<sal_uInt16[]> pMetrics = GetTTSimpleGlyphMetrics( pTTFont,
                                                                  pGID,
                                                                  nGlyphs,
                                                                  false/*bVertical*/ );
        if( pMetrics )
        {
            for( int i = 0; i < nGlyphs; i++ )
                pWidths[pOldIndex[i]] = pMetrics[i];
            pMetrics.reset();
        }
        else
        {
            CloseTTFont( pTTFont );
            return false;
        }
    }

    bool bSuccess = ( SFErrCodes::Ok == CreateTTFromTTGlyphs( pTTFont,
                                                     aToFile.getStr(),
                                                     pGID,
                                                     pEnc,
                                                     nGlyphs ) );
    CloseTTFont( pTTFont );

    return bSuccess;
}

void PrintFontManager::getGlyphWidths( fontID nFont,
                                       bool bVertical,
                                       std::vector< sal_Int32 >& rWidths,
                                       std::map< sal_Unicode, sal_uInt32 >& rUnicodeEnc )
{
    PrintFont* pFont = getFont( nFont );
    if (!pFont)
        return;
    TrueTypeFont* pTTFont = nullptr;
    OString aFromFile = getFontFile( *pFont );
    if( OpenTTFontFile( aFromFile.getStr(), pFont->m_nCollectionEntry, &pTTFont ) != SFErrCodes::Ok )
        return;
    int nGlyphs = pTTFont->glyphCount();
    if (nGlyphs > 0)
    {
        rWidths.resize(nGlyphs);
        std::vector<sal_uInt16> aGlyphIds(nGlyphs);
        for (int i = 0; i < nGlyphs; i++)
            aGlyphIds[i] = sal_uInt16(i);
        std::unique_ptr<sal_uInt16[]> pMetrics = GetTTSimpleGlyphMetrics(pTTFont,
                                                                 aGlyphIds.data(),
                                                                 nGlyphs,
                                                                 bVertical);
        if (pMetrics)
        {
            for (int i = 0; i< nGlyphs; i++)
                rWidths[i] = pMetrics[i];
            pMetrics.reset();
            rUnicodeEnc.clear();
        }

        // fill the unicode map
        // TODO: isn't this map already available elsewhere in the fontmanager?
        sal_uInt32 nCmapSize = 0;
        const sal_uInt8* pCmapData = pTTFont->table(O_cmap, nCmapSize);
        if (pCmapData)
        {
            CmapResult aCmapResult;
            if (ParseCMAP(pCmapData, nCmapSize, aCmapResult))
            {
                FontCharMapRef xFontCharMap(new FontCharMap(aCmapResult));
                for (sal_uInt32 cOld = 0;;)
                {
                    // get next unicode covered by font
                    const sal_uInt32 c = xFontCharMap->GetNextChar(cOld);
                    if (c == cOld)
                        break;
                    cOld = c;
#if 1 // TODO: remove when sal_Unicode covers all of unicode
                    if (c > sal_Unicode(~0))
                        break;
#endif
                    // get the matching glyph index
                    const sal_GlyphId aGlyphId = xFontCharMap->GetGlyphIndex(c);
                    // update the requested map
                    rUnicodeEnc[static_cast<sal_Unicode>(c)] = aGlyphId;
                }
            }
        }
    }
    CloseTTFont(pTTFont);
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
