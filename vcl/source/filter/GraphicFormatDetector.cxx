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

#include <sal/config.h>

#include <algorithm>

#include <graphic/GraphicFormatDetector.hxx>
#include <graphic/DetectorTools.hxx>
#include <tools/solar.h>
#include <tools/zcodec.hxx>
#include <utility>

constexpr sal_uInt32 SVG_CHECK_SIZE = 2048;
constexpr sal_uInt32 WMF_EMF_CHECK_SIZE = 44;

namespace vcl
{
bool peekGraphicFormat(SvStream& rStream, OUString& rFormatExtension, bool bTest)
{
    vcl::GraphicFormatDetector aDetector(rStream, rFormatExtension);
    if (!aDetector.detect())
        return false;

    // The following variable is used when bTest == true. It remains false
    // if the format (rFormatExtension) has not yet been set.
    bool bSomethingTested = false;

    // Now the different formats are checked. The order *does* matter. e.g. a MET file
    // could also go through the BMP test, however, a BMP file can hardly go through the MET test.
    // So MET should be tested prior to BMP. However, theoretically a BMP file could conceivably
    // go through the MET test. These problems are of course not only in MET and BMP.
    // Therefore, in the case of a format check (bTest == true)  we only test *exactly* this
    // format. Everything else could have fatal consequences, for example if the user says it is
    // a BMP file (and it is a BMP) file, and the file would go through the MET test ...

    if (!bTest || rFormatExtension.startsWith("MET"))
    {
        bSomethingTested = true;
        if (aDetector.checkMET())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("BMP"))
    {
        bSomethingTested = true;
        if (aDetector.checkBMP())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("WMF") || rFormatExtension.startsWith("EMF")
        || rFormatExtension.startsWith("WMZ") || rFormatExtension.startsWith("EMZ"))
    {
        bSomethingTested = true;
        if (aDetector.checkWMForEMF())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("PCX"))
    {
        bSomethingTested = true;
        if (aDetector.checkPCX())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("TIF"))
    {
        bSomethingTested = true;
        if (aDetector.checkTIF())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("GIF"))
    {
        bSomethingTested = true;
        if (aDetector.checkGIF())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("PNG"))
    {
        bSomethingTested = true;
        if (aDetector.checkPNG())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("JPG"))
    {
        bSomethingTested = true;
        if (aDetector.checkJPG())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("SVM"))
    {
        bSomethingTested = true;
        if (aDetector.checkSVM())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("PCD"))
    {
        bSomethingTested = true;
        if (aDetector.checkPCD())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("PSD"))
    {
        bSomethingTested = true;
        if (aDetector.checkPSD())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("EPS"))
    {
        bSomethingTested = true;
        if (aDetector.checkEPS())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("DXF"))
    {
        if (aDetector.checkDXF())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("PCT"))
    {
        bSomethingTested = true;
        if (aDetector.checkPCT())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("PBM") || rFormatExtension.startsWith("PGM")
        || rFormatExtension.startsWith("PPM"))
    {
        bSomethingTested = true;
        if (aDetector.checkPBMorPGMorPPM())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("RAS"))
    {
        bSomethingTested = true;
        if (aDetector.checkRAS())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest)
    {
        bSomethingTested = true;
        if (aDetector.checkXPM())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }
    else if (rFormatExtension.startsWith("XPM"))
    {
        return true;
    }

    if (!bTest)
    {
        if (aDetector.checkXBM())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }
    else if (rFormatExtension.startsWith("XBM"))
    {
        return true;
    }

    if (!bTest)
    {
        if (aDetector.checkSVG())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }
    else if (rFormatExtension.startsWith("SVG"))
    {
        return true;
    }

    if (!bTest || rFormatExtension.startsWith("TGA"))
    {
        bSomethingTested = true;
        if (aDetector.checkTGA())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("MOV"))
    {
        if (aDetector.checkMOV())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("PDF"))
    {
        if (aDetector.checkPDF())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    if (!bTest || rFormatExtension.startsWith("WEBP"))
    {
        bSomethingTested = true;
        if (aDetector.checkWEBP())
        {
            rFormatExtension = aDetector.msDetectedFormat;
            return true;
        }
    }

    return bTest && !bSomethingTested;
}

namespace
{
bool isPCT(SvStream& rStream, sal_uLong nStreamPos, sal_uLong nStreamLen)
{
    sal_uInt8 sBuf[3];
    // store number format
    SvStreamEndian oldNumberFormat = rStream.GetEndian();
    sal_uInt32 nOffset; // in MS documents the pict format is used without the first 512 bytes
    for (nOffset = 0; (nOffset <= 512) && ((nStreamPos + nOffset + 14) <= nStreamLen);
         nOffset += 512)
    {
        short y1, x1, y2, x2;
        bool bdBoxOk = true;

        rStream.Seek(nStreamPos + nOffset);
        // size of the pict in version 1 pict ( 2bytes) : ignored
        rStream.SeekRel(2);
        // bounding box (bytes 2 -> 9)
        rStream.SetEndian(SvStreamEndian::BIG);
        rStream.ReadInt16(y1).ReadInt16(x1).ReadInt16(y2).ReadInt16(x2);
        rStream.SetEndian(oldNumberFormat); // reset format

        // read version op
        rStream.ReadBytes(sBuf, 3);

        if (!rStream.good())
            break;

        if (x1 > x2 || y1 > y2 || // bad bdbox
            (x1 == x2 && y1 == y2) || // 1 pixel picture
            x2 - x1 > 2048 || y2 - y1 > 2048) // picture abnormally big
            bdBoxOk = false;

        // see http://developer.apple.com/legacy/mac/library/documentation/mac/pdf/Imaging_With_QuickDraw/Appendix_A.pdf
        // normal version 2 - page A23 and A24
        if (sBuf[0] == 0x00 && sBuf[1] == 0x11 && sBuf[2] == 0x02)
            return true;
        // normal version 1 - page A25
        else if (sBuf[0] == 0x11 && sBuf[1] == 0x01 && bdBoxOk)
            return true;
    }
    return false;
}

} // end anonymous namespace

GraphicFormatDetector::GraphicFormatDetector(SvStream& rStream, OUString aFormatExtension)
    : mrStream(rStream)
    , maExtension(std::move(aFormatExtension))
    , mnFirstLong(0)
    , mnSecondLong(0)
    , mnStreamPosition(0)
    , mnStreamLength(0)
{
}

bool GraphicFormatDetector::detect()
{
    maFirstBytes.clear();
    maFirstBytes.resize(256, 0);

    mnFirstLong = 0;
    mnSecondLong = 0;

    mnStreamPosition = mrStream.Tell();
    mnStreamLength = mrStream.remainingSize();

    if (!mnStreamLength)
    {
        SvLockBytes* pLockBytes = mrStream.GetLockBytes();
        if (pLockBytes)
            pLockBytes->SetSynchronMode();
        mnStreamLength = mrStream.remainingSize();
    }

    if (mnStreamLength == 0)
    {
        return false; // this prevents at least a STL assertion
    }
    else if (mnStreamLength >= maFirstBytes.size())
    {
        // load first 256 bytes into a buffer
        sal_uInt64 nRead = mrStream.ReadBytes(maFirstBytes.data(), maFirstBytes.size());
        if (nRead < maFirstBytes.size())
            mnStreamLength = nRead;
    }
    else
    {
        mnStreamLength = mrStream.ReadBytes(maFirstBytes.data(), mnStreamLength);
    }

    if (mrStream.GetError())
        return false;

    for (int i = 0; i < 4; ++i)
    {
        mnFirstLong = (mnFirstLong << 8) | sal_uInt32(maFirstBytes[i]);
        mnSecondLong = (mnSecondLong << 8) | sal_uInt32(maFirstBytes[i + 4]);
    }
    return true;
}

bool GraphicFormatDetector::checkMET()
{
    if (maFirstBytes[2] != 0xd3)
        return false;
    mrStream.SetEndian(SvStreamEndian::BIG);
    mrStream.Seek(mnStreamPosition);
    sal_uInt16 nFieldSize;
    sal_uInt8 nMagic;

    mrStream.ReadUInt16(nFieldSize).ReadUChar(nMagic);
    for (int i = 0; i < 3; i++)
    {
        if (nFieldSize < 6)
            return false;
        if (mnStreamLength < mrStream.Tell() + nFieldSize)
            return false;
        mrStream.SeekRel(nFieldSize - 3);
        mrStream.ReadUInt16(nFieldSize).ReadUChar(nMagic);
        if (nMagic != 0xd3)
            return false;
    }
    mrStream.SetEndian(SvStreamEndian::LITTLE);

    if (mrStream.GetError())
        return false;

    msDetectedFormat = "MET";
    return true;
}

bool GraphicFormatDetector::checkBMP()
{
    sal_uInt8 nOffset;

    // We're possibly also able to read an OS/2 bitmap array
    // ('BA'), therefore we must adjust the offset to discover the
    // first bitmap in the array
    if (maFirstBytes[0] == 0x42 && maFirstBytes[1] == 0x41)
        nOffset = 14;
    else
        nOffset = 0;

    // Now we initially test on 'BM'
    if (maFirstBytes[0 + nOffset] == 0x42 && maFirstBytes[1 + nOffset] == 0x4d)
    {
        // OS/2 can set the Reserved flags to a value other than 0
        // (which they really should not do...);
        // In this case we test the size of the BmpInfoHeaders
        if ((maFirstBytes[6 + nOffset] == 0x00 && maFirstBytes[7 + nOffset] == 0x00
             && maFirstBytes[8 + nOffset] == 0x00 && maFirstBytes[9 + nOffset] == 0x00)
            || maFirstBytes[14 + nOffset] == 0x28 || maFirstBytes[14 + nOffset] == 0x0c)
        {
            msDetectedFormat = "BMP";
            return true;
        }
    }
    return false;
}

bool GraphicFormatDetector::checkWMForEMF()
{
    sal_uInt64 nCheckSize = std::min<sal_uInt64>(mnStreamLength, 256);
    sal_uInt8 sExtendedOrDecompressedFirstBytes[WMF_EMF_CHECK_SIZE];
    sal_uInt64 nDecompressedSize = nCheckSize;
    // check if it is gzipped -> wmz/emz
    sal_uInt8* pCheckArray = checkAndUncompressBuffer(sExtendedOrDecompressedFirstBytes,
                                                      WMF_EMF_CHECK_SIZE, nDecompressedSize);
    if (mnFirstLong == 0xd7cdc69a || mnFirstLong == 0x01000900)
    {
        msDetectedFormat = "WMF";
        return true;
    }
    else if (mnFirstLong == 0x01000000 && pCheckArray[40] == 0x20 && pCheckArray[41] == 0x45
             && pCheckArray[42] == 0x4d && pCheckArray[43] == 0x46)
    {
        msDetectedFormat = "EMF";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkPCX()
{
    if (maFirstBytes[0] != 0x0a)
        return false;

    sal_uInt8 nVersion = maFirstBytes[1];
    sal_uInt8 nEncoding = maFirstBytes[2];
    if ((nVersion == 0 || nVersion == 2 || nVersion == 3 || nVersion == 5) && nEncoding <= 1)
    {
        msDetectedFormat = "PCX";
        return true;
    }

    return false;
}

bool GraphicFormatDetector::checkTIF()
{
    if (mnFirstLong == 0x49492a00 || mnFirstLong == 0x4d4d002a)
    {
        msDetectedFormat = "TIF";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkGIF()
{
    if (mnFirstLong == 0x47494638 && (maFirstBytes[4] == 0x37 || maFirstBytes[4] == 0x39)
        && maFirstBytes[5] == 0x61)
    {
        msDetectedFormat = "GIF";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkPNG()
{
    if (mnFirstLong == 0x89504e47 && mnSecondLong == 0x0d0a1a0a)
    {
        msDetectedFormat = "PNG";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkJPG()
{
    if ((mnFirstLong == 0xffd8ffe0 && maFirstBytes[6] == 0x4a && maFirstBytes[7] == 0x46
         && maFirstBytes[8] == 0x49 && maFirstBytes[9] == 0x46)
        || (mnFirstLong == 0xffd8fffe) || (0xffd8ff00 == (mnFirstLong & 0xffffff00)))
    {
        msDetectedFormat = "JPG";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkSVM()
{
    if (mnFirstLong == 0x53564744 && maFirstBytes[4] == 0x49)
    {
        msDetectedFormat = "SVM";
        return true;
    }
    else if (maFirstBytes[0] == 0x56 && maFirstBytes[1] == 0x43 && maFirstBytes[2] == 0x4C
             && maFirstBytes[3] == 0x4D && maFirstBytes[4] == 0x54 && maFirstBytes[5] == 0x46)
    {
        msDetectedFormat = "SVM";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkPCD()
{
    if (mnStreamLength < 2055)
        return false;
    char sBuffer[8];
    mrStream.Seek(mnStreamPosition + 2048);
    sBuffer[mrStream.ReadBytes(sBuffer, 7)] = 0;

    if (strncmp(sBuffer, "PCD_IPI", 7) == 0)
    {
        msDetectedFormat = "PCD";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkPSD()
{
    if ((mnFirstLong == 0x38425053) && ((mnSecondLong >> 16) == 1))
    {
        msDetectedFormat = "PSD";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkEPS()
{
    const char* pFirstBytesAsCharArray = reinterpret_cast<char*>(maFirstBytes.data());

    if (mnFirstLong == 0xC5D0D3C6)
    {
        msDetectedFormat = "EPS";
        return true;
    }
    else if (checkArrayForMatchingStrings(pFirstBytesAsCharArray, 30, { "%!PS-Adobe", " EPS" }))
    {
        msDetectedFormat = "EPS";
        return true;
    }

    return false;
}

bool GraphicFormatDetector::checkDXF()
{
    if (strncmp(reinterpret_cast<char*>(maFirstBytes.data()), "AutoCAD Binary DXF", 18) == 0)
    {
        msDetectedFormat = "DXF";
        return true;
    }

    // ASCII DXF File Format
    int i = 0;
    while (i < 256 && maFirstBytes[i] <= 32)
    {
        ++i;
    }

    if (i < 256 && maFirstBytes[i] == '0')
    {
        ++i;

        // only now do we have sufficient data to make a judgement
        // based on a '0' + 'SECTION' == DXF argument

        while (i < 256 && maFirstBytes[i] <= 32)
        {
            ++i;
        }

        if (i + 7 < 256
            && (strncmp(reinterpret_cast<char*>(maFirstBytes.data() + i), "SECTION", 7) == 0))
        {
            msDetectedFormat = "DXF";
            return true;
        }
    }
    return false;
}

bool GraphicFormatDetector::checkPCT()
{
    if (isPCT(mrStream, mnStreamPosition, mnStreamLength))
    {
        msDetectedFormat = "PCT";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkPBMorPGMorPPM()
{
    if (maFirstBytes[0] == 'P')
    {
        switch (maFirstBytes[1])
        {
            case '1':
            case '4':
                msDetectedFormat = "PBM";
                return true;

            case '2':
            case '5':
                msDetectedFormat = "PGM";
                return true;

            case '3':
            case '6':
                msDetectedFormat = "PPM";
                return true;
        }
    }
    return false;
}

bool GraphicFormatDetector::checkRAS()
{
    if (mnFirstLong == 0x59a66a95)
    {
        msDetectedFormat = "RAS";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkXPM()
{
    const char* pFirstBytesAsCharArray = reinterpret_cast<char*>(maFirstBytes.data());
    if (matchArrayWithString(pFirstBytesAsCharArray, 256, "/* XPM */"))
    {
        msDetectedFormat = "XPM";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkXBM()
{
    sal_uInt64 nSize = std::min<sal_uInt64>(mnStreamLength, 2048);
    std::unique_ptr<sal_uInt8[]> pBuffer(new sal_uInt8[nSize]);

    mrStream.Seek(mnStreamPosition);
    nSize = mrStream.ReadBytes(pBuffer.get(), nSize);

    const char* pBufferAsCharArray = reinterpret_cast<char*>(pBuffer.get());

    if (checkArrayForMatchingStrings(pBufferAsCharArray, nSize, { "#define", "_width" }))
    {
        msDetectedFormat = "XBM";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkSVG()
{
    sal_uInt64 nCheckSize = std::min<sal_uInt64>(mnStreamLength, 256);
    sal_uInt8 sExtendedOrDecompressedFirstBytes[SVG_CHECK_SIZE];
    sal_uInt64 nDecompressedSize = nCheckSize;
    // check if it is gzipped -> svgz
    sal_uInt8* pCheckArray = checkAndUncompressBuffer(sExtendedOrDecompressedFirstBytes,
                                                      SVG_CHECK_SIZE, nDecompressedSize);
    nCheckSize = std::min<sal_uInt64>(nDecompressedSize, 256);
    bool bIsSvg(false);
    bool bIsGZip = (nDecompressedSize > 0);
    const char* pCheckArrayAsCharArray = reinterpret_cast<char*>(pCheckArray);
    // check for XML
    // #119176# SVG files which have no xml header at all have shown up this is optional
    // check for "xml" then "version" then "DOCTYPE" and "svg" tags
    if (checkArrayForMatchingStrings(pCheckArrayAsCharArray, nCheckSize,
                                     { "<?xml", "version", "DOCTYPE", "svg" }))
    {
        bIsSvg = true;
    }

    // check for svg element in 1st 256 bytes
    // search for '<svg'
    if (!bIsSvg && checkArrayForMatchingStrings(pCheckArrayAsCharArray, nCheckSize, { "<svg" }))
    {
        bIsSvg = true;
    }

    // extended search for svg element
    if (!bIsSvg)
    {
        // it's a xml, look for '<svg' in full file. Should not happen too
        // often since the tests above will handle most cases, but can happen
        // with Svg files containing big comment headers or Svg as the host
        // language

        pCheckArrayAsCharArray = reinterpret_cast<char*>(sExtendedOrDecompressedFirstBytes);

        if (bIsGZip)
        {
            nCheckSize = std::min<sal_uInt64>(nDecompressedSize, 2048);
        }
        else
        {
            nCheckSize = std::min<sal_uInt64>(mnStreamLength, 2048);
            mrStream.Seek(mnStreamPosition);
            nCheckSize = mrStream.ReadBytes(sExtendedOrDecompressedFirstBytes, nCheckSize);
        }

        // search for '<svg'
        if (checkArrayForMatchingStrings(pCheckArrayAsCharArray, nCheckSize, { "<svg" }))
        {
            bIsSvg = true;
        }
    }

    if (bIsSvg)
    {
        msDetectedFormat = "SVG";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkTGA()
{
    // Check TGA ver.2 footer bytes
    if (mnStreamLength > 18)
    {
        char sFooterBytes[18];

        mrStream.Seek(STREAM_SEEK_TO_END);
        mrStream.SeekRel(-18);
        if (mrStream.ReadBytes(sFooterBytes, 18) == 18
            && memcmp(sFooterBytes, "TRUEVISION-XFILE.", SAL_N_ELEMENTS(sFooterBytes)) == 0)
        {
            msDetectedFormat = "TGA";
            return true;
        }
    }

    // Fallback to file extension check
    if (maExtension.startsWith("TGA"))
    {
        msDetectedFormat = "TGA";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkMOV()
{
    if ((maFirstBytes[4] == 'f' && maFirstBytes[5] == 't' && maFirstBytes[6] == 'y'
         && maFirstBytes[7] == 'p' && maFirstBytes[8] == 'q' && maFirstBytes[9] == 't')
        || (maFirstBytes[4] == 'm' && maFirstBytes[5] == 'o' && maFirstBytes[6] == 'o'
            && maFirstBytes[7] == 'v' && maFirstBytes[11] == 'l' && maFirstBytes[12] == 'm'))
    {
        msDetectedFormat = "MOV";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkPDF()
{
    if (maFirstBytes[0] == '%' && maFirstBytes[1] == 'P' && maFirstBytes[2] == 'D'
        && maFirstBytes[3] == 'F' && maFirstBytes[4] == '-')
    {
        msDetectedFormat = "PDF";
        return true;
    }
    return false;
}

bool GraphicFormatDetector::checkWEBP()
{
    if (maFirstBytes[0] == 'R' && maFirstBytes[1] == 'I' && maFirstBytes[2] == 'F'
        && maFirstBytes[3] == 'F' && maFirstBytes[8] == 'W' && maFirstBytes[9] == 'E'
        && maFirstBytes[10] == 'B' && maFirstBytes[11] == 'P')
    {
        msDetectedFormat = "WEBP";
        return true;
    }
    return false;
}

sal_uInt8* GraphicFormatDetector::checkAndUncompressBuffer(sal_uInt8* aUncompressedBuffer,
                                                           sal_uInt32 nSize, sal_uInt64& nRetSize)
{
    if (ZCodec::IsZCompressed(mrStream))
    {
        ZCodec aCodec;
        mrStream.Seek(mnStreamPosition);
        aCodec.BeginCompression(ZCODEC_DEFAULT_COMPRESSION, /*gzLib*/ true);
        auto nDecompressedOut = aCodec.Read(mrStream, aUncompressedBuffer, nSize);
        // ZCodec::Decompress returns -1 on failure
        nRetSize = nDecompressedOut < 0 ? 0 : nDecompressedOut;
        aCodec.EndCompression();
        // Recalculate first/second long
        for (int i = 0; i < 4; ++i)
        {
            mnFirstLong = (mnFirstLong << 8) | sal_uInt32(aUncompressedBuffer[i]);
            mnSecondLong = (mnSecondLong << 8) | sal_uInt32(aUncompressedBuffer[i + 4]);
        }
        return aUncompressedBuffer;
    }
    nRetSize = 0;
    return maFirstBytes.data();
}

} // vcl namespace

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
