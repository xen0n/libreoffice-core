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

#include <DocumentSettingManager.hxx> //For SwFmt::getIDocumentSettingAccess()
#include <IDocumentTimerAccess.hxx>
#include <doc.hxx>
#include <fmtcolfunc.hxx>
#include <format.hxx>
#include <frmatr.hxx>
#include <hintids.hxx>
#include <hints.hxx>
#include <o3tl/unit_conversion.hxx>
#include <osl/diagnose.h>
#include <sal/log.hxx>
#include <svl/grabbagitem.hxx>
#include <svx/sdr/attribute/sdrallfillattributeshelper.hxx>
#include <svx/unobrushitemhelper.hxx>
#include <svx/xdef.hxx>

using namespace com::sun::star;

SwFormat::SwFormat( SwAttrPool& rPool, const char* pFormatNm,
              const WhichRangesContainer& pWhichRanges, SwFormat *pDrvdFrame,
              sal_uInt16 nFormatWhich ) :
    m_aFormatName( OUString::createFromAscii(pFormatNm) ),
    m_aSet( rPool, pWhichRanges ),
    m_nWhichId( nFormatWhich ),
    m_nPoolFormatId( USHRT_MAX ),
    m_nPoolHelpId( USHRT_MAX ),
    m_nPoolHlpFileId( UCHAR_MAX )
{
    m_bAutoUpdateFormat = false; // LAYER_IMPL
    m_bAutoFormat = true;
    m_bFormatInDTOR = m_bHidden = false;

    if( pDrvdFrame )
    {
        pDrvdFrame->Add(this);
        m_aSet.SetParent( &pDrvdFrame->m_aSet );
    }
}

SwFormat::SwFormat( SwAttrPool& rPool, const OUString& rFormatNm,
              const WhichRangesContainer& pWhichRanges, SwFormat* pDrvdFrame,
              sal_uInt16 nFormatWhich ) :
    m_aFormatName( rFormatNm ),
    m_aSet( rPool, pWhichRanges ),
    m_nWhichId( nFormatWhich ),
    m_nPoolFormatId( USHRT_MAX ),
    m_nPoolHelpId( USHRT_MAX ),
    m_nPoolHlpFileId( UCHAR_MAX )
{
    m_bAutoUpdateFormat = false; // LAYER_IMPL
    m_bAutoFormat = true;
    m_bFormatInDTOR = m_bHidden = false;

    if( pDrvdFrame )
    {
        pDrvdFrame->Add(this);
        m_aSet.SetParent( &pDrvdFrame->m_aSet );
    }
}

SwFormat::SwFormat( const SwFormat& rFormat ) :
    sw::BorderCacheOwner(),
    m_aFormatName( rFormat.m_aFormatName ),
    m_aSet( rFormat.m_aSet ),
    m_nWhichId( rFormat.m_nWhichId ),
    m_nPoolFormatId( rFormat.GetPoolFormatId() ),
    m_nPoolHelpId( rFormat.GetPoolHelpId() ),
    m_nPoolHlpFileId( rFormat.GetPoolHlpFileId() )
{
    m_bFormatInDTOR = false; // LAYER_IMPL
    m_bAutoFormat = rFormat.m_bAutoFormat;
    m_bHidden = rFormat.m_bHidden;
    m_bAutoUpdateFormat = rFormat.m_bAutoUpdateFormat;

    if( auto pDerived = rFormat.DerivedFrom() )
    {
        pDerived->Add(this);
        m_aSet.SetParent( &pDerived->m_aSet );
    }
    // a few special treatments for attributes
    m_aSet.SetModifyAtAttr( this );
}

SwFormat &SwFormat::operator=(const SwFormat& rFormat)
{
    if(this == &rFormat)
        return *this;

    m_nWhichId = rFormat.m_nWhichId;
    m_nPoolFormatId = rFormat.GetPoolFormatId();
    m_nPoolHelpId = rFormat.GetPoolHelpId();
    m_nPoolHlpFileId = rFormat.GetPoolHlpFileId();

    InvalidateInSwCache(RES_OBJECTDYING);

    // copy only array with attributes delta
    SwAttrSet aOld( *m_aSet.GetPool(), m_aSet.GetRanges() ),
              aNew( *m_aSet.GetPool(), m_aSet.GetRanges() );
    m_aSet.Intersect_BC( rFormat.m_aSet, &aOld, &aNew );
    (void)m_aSet.Put_BC( rFormat.m_aSet, &aOld, &aNew );

    // a few special treatments for attributes
    m_aSet.SetModifyAtAttr( this );

    // create PoolItem attribute for Modify
    if( aOld.Count() )
    {
        sw::ClientNotifyAttrChg(*this, m_aSet, aOld, aNew);
    }

    if(GetRegisteredIn() != rFormat.GetRegisteredIn())
    {
        StartListeningToSameModifyAs(rFormat);
        m_aSet.SetParent( GetRegisteredIn()
            ? &rFormat.m_aSet
            : nullptr);
    }

    m_bAutoFormat = rFormat.m_bAutoFormat;
    m_bHidden = rFormat.m_bHidden;
    m_bAutoUpdateFormat = rFormat.m_bAutoUpdateFormat;
    return *this;
}

void SwFormat::SetName( const OUString& rNewName, bool bBroadcast )
{
    OSL_ENSURE( !IsDefault(), "SetName: Defaultformat" );
    if( bBroadcast )
    {
        SwStringMsgPoolItem aOld( RES_NAME_CHANGED, m_aFormatName );
        SwStringMsgPoolItem aNew( RES_NAME_CHANGED, rNewName );
        m_aFormatName = rNewName;
        const sw::LegacyModifyHint aHint(&aOld, &aNew);
        SwClientNotify(*this, aHint);
    }
    else
    {
        m_aFormatName = rNewName;
    }
}

/** Copy attributes

    This function is called in every Copy-Ctor for copying the attributes.
    The latter can be only copied as soon as the derived class exists since
    for setting them the Which() function is called and that has the default
    value of 0 in the base class and is then overridden by the derived class.

    If we copy over multiple documents then the new document has to be provided
    in which <this> is defined. Currently this is important for DropCaps
    because that contains data that needs to be copied deeply.
*/
void SwFormat::CopyAttrs( const SwFormat& rFormat )
{
    // copy only array with attributes delta
    InvalidateInSwCache(RES_ATTRSET_CHG);
    InvalidateInSwFntCache(RES_ATTRSET_CHG);

    // special treatments for some attributes
    SwAttrSet* pChgSet = const_cast<SwAttrSet*>(&rFormat.m_aSet);

    // copy only array with attributes delta
    if( pChgSet->GetPool() != m_aSet.GetPool() )
        pChgSet->CopyToModify( *this );
    else
    {
        SwAttrSet aOld( *m_aSet.GetPool(), m_aSet.GetRanges() ),
                  aNew( *m_aSet.GetPool(), m_aSet.GetRanges() );

        if ( m_aSet.Put_BC( *pChgSet, &aOld, &aNew ) )
        {
            // a few special treatments for attributes
            m_aSet.SetModifyAtAttr( this );

            SwAttrSetChg aChgOld( m_aSet, aOld );
            SwAttrSetChg aChgNew( m_aSet, aNew );
            SwClientNotify(*this, sw::LegacyModifyHint(&aChgOld, &aChgNew)); // send all modified ones
        }
    }

    if( pChgSet != &rFormat.m_aSet ) // was a Set created?
        delete pChgSet;
}

SwFormat::~SwFormat()
{
    // This happens at an ObjectDying message. Thus put all dependent
    // ones on DerivedFrom.
    if(!HasWriterListeners())
        return;

    m_bFormatInDTOR = true;
    
    if(!DerivedFrom())
    {
        SwFormat::ResetFormatAttr(RES_PAGEDESC);
        SAL_WARN("sw.core", "~SwFormat: format still has clients on death, but parent format is missing: " << GetName());
        return;
    }
    SwIterator<SwClient,SwFormat> aIter(*this);
    for(SwClient* pClient = aIter.First(); pClient; pClient = aIter.Next())
        pClient->CheckRegistrationFormat(*this);
    assert(!HasWriterListeners());
}

void SwFormat::SwClientNotify(const SwModify&, const SfxHint& rHint)
{
    if (rHint.GetId() != SfxHintId::SwLegacyModify)
        return;
    auto pLegacy = static_cast<const sw::LegacyModifyHint*>(&rHint);

    std::unique_ptr<SwAttrSetChg> pOldClientChg, pNewClientChg;
    auto pDependsHint = std::make_unique<sw::LegacyModifyHint>(pLegacy->m_pOld, pLegacy->m_pNew);
    const sal_uInt16 nWhich = pLegacy->GetWhich();
    InvalidateInSwCache(nWhich);
    switch(nWhich)
    {
        case 0:
            break;
        case RES_OBJECTDYING:
        {
            // NB: this still notifies depends even if pLegacy->m_pNew is nullptr, which seems non-obvious
            if(!pLegacy->m_pNew)
                break;
            // If the dying object is the parent format of this format so
            // attach this to the parent of the parent
            SwFormat* pFormat = static_cast<SwFormat*>(pLegacy->m_pNew->StaticWhichCast(RES_OBJECTDYING).pObject);

            // do not move if this is the topmost format
            if(GetRegisteredIn() && GetRegisteredIn() == pFormat)
            {
                if(pFormat->GetRegisteredIn())
                {
                    // if parent so register in new parent
                    pFormat->DerivedFrom()->Add(this);
                    m_aSet.SetParent(&DerivedFrom()->m_aSet);
                }
                else
                {
                    // otherwise de-register at least from dying one
                    EndListeningAll();
                    m_aSet.SetParent(nullptr);
                }
            }
            break;
        }
        case RES_ATTRSET_CHG:
        {
            // NB: this still notifies depends even if this condition is not met, which seems non-obvious
            auto pOldAttrSetChg = static_cast<const SwAttrSetChg*>(pLegacy->m_pOld);
            auto pNewAttrSetChg = static_cast<const SwAttrSetChg*>(pLegacy->m_pNew);
            if (pOldAttrSetChg && pNewAttrSetChg && pOldAttrSetChg->GetTheChgdSet() != &m_aSet)
            {
                // pass only those that are not set...
                pNewClientChg.reset(new SwAttrSetChg(*pNewAttrSetChg));
                pNewClientChg->GetChgSet()->Differentiate(m_aSet);
                if(pNewClientChg->Count()) // ... if any
                {
                    pOldClientChg.reset(new SwAttrSetChg(*pOldAttrSetChg));
                    pOldClientChg->GetChgSet()->Differentiate(m_aSet);
                    pDependsHint.reset(new sw::LegacyModifyHint(pOldClientChg.get(), pNewClientChg.get()));
                }
                else
                    pDependsHint.reset(nullptr);
            }
            break;
        }
        case RES_FMT_CHG:
        {
            // if the format parent will be moved so register my attribute set at
            // the new one

            // skip my own Modify
            // NB: this still notifies depends even if this condition is not met, which seems non-obvious
            auto pOldFormatChg = static_cast<const SwFormatChg*>(pLegacy->m_pOld);
            auto pNewFormatChg = static_cast<const SwFormatChg*>(pLegacy->m_pNew);
            if(pOldFormatChg && pNewFormatChg && pOldFormatChg->pChangedFormat != this && pNewFormatChg->pChangedFormat == GetRegisteredIn())
            {
                // attach Set to new parent
                m_aSet.SetParent(DerivedFrom() ? &DerivedFrom()->m_aSet : nullptr);
            }
            break;
        }
        default:
            // attribute is defined in this format
            if(SfxItemState::SET == m_aSet.GetItemState(nWhich, false))
            {
                // DropCaps might come into this block
                SAL_WARN_IF(RES_PARATR_DROP != nWhich, "sw.core", "Hint was sent without sender");
                pDependsHint.reset(nullptr);
            }
    }
    if(pDependsHint)
    {
        InvalidateInSwFntCache(pDependsHint->GetWhich());
        SwModify::SwClientNotify(*this, *pDependsHint);
    }
}

bool SwFormat::SetDerivedFrom(SwFormat *pDerFrom)
{
    if ( pDerFrom )
    {
        const SwFormat* pFormat = pDerFrom;
        while ( pFormat != nullptr )
        {
            if ( pFormat == this )
                return false;

            pFormat=pFormat->DerivedFrom();
        }
    }
    else
    {
        // nothing provided, search for Dflt format
        pDerFrom = this;
        while ( pDerFrom->DerivedFrom() )
            pDerFrom = pDerFrom->DerivedFrom();
    }
    if ( (pDerFrom == DerivedFrom()) || (pDerFrom == this) )
        return false;

    assert(    Which()==pDerFrom->Which()
            || (Which()==RES_CONDTXTFMTCOLL && pDerFrom->Which()==RES_TXTFMTCOLL)
            || (Which()==RES_TXTFMTCOLL && pDerFrom->Which()==RES_CONDTXTFMTCOLL)
            || (Which()==RES_FLYFRMFMT && pDerFrom->Which()==RES_FRMFMT)
            );

    InvalidateInSwCache(RES_ATTRSET_CHG);
    InvalidateInSwFntCache(RES_ATTRSET_CHG);

    pDerFrom->Add( this );
    m_aSet.SetParent( &pDerFrom->m_aSet );

    SwFormatChg aOldFormat( this );
    SwFormatChg aNewFormat( this );
    const sw::LegacyModifyHint aHint(&aOldFormat, &aNewFormat);
    SwClientNotify(*this, aHint);

    return true;
}

bool SwFormat::supportsFullDrawingLayerFillAttributeSet() const
{
    return false;
}

const SfxPoolItem& SwFormat::GetFormatAttr( sal_uInt16 nWhich, bool bInParents ) const
{
    if (RES_BACKGROUND == nWhich && supportsFullDrawingLayerFillAttributeSet())
    {
        // FALLBACKBREAKHERE should not be used; instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST]
        SAL_INFO("sw.core", "Do no longer use SvxBrushItem, instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST] FillAttributes or makeBackgroundBrushItem (simple fallback is in place and used)");
        static std::unique_ptr<SvxBrushItem> aSvxBrushItem; //(std::make_shared<SvxBrushItem>(RES_BACKGROUND));

        // fill the local static SvxBrushItem from the current ItemSet so that
        // the fill attributes [XATTR_FILL_FIRST .. XATTR_FILL_LAST] are used
        // as good as possible to create a fallback representation and return that
        aSvxBrushItem = getSvxBrushItemFromSourceSet(m_aSet, RES_BACKGROUND, bInParents);

        return *aSvxBrushItem;
    }

    return m_aSet.Get( nWhich, bInParents );
}

SfxItemState SwFormat::GetItemState( sal_uInt16 nWhich, bool bSrchInParent, const SfxPoolItem **ppItem ) const
{
    if (RES_BACKGROUND == nWhich && supportsFullDrawingLayerFillAttributeSet())
    {
        // FALLBACKBREAKHERE should not be used; instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST]
        SAL_INFO("sw.core", "Do no longer use SvxBrushItem, instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST] FillAttributes or SwFormat::GetBackgroundStat (simple fallback is in place and used)");
        const drawinglayer::attribute::SdrAllFillAttributesHelperPtr aFill = getSdrAllFillAttributesHelper();

        // check if the new fill attributes are used
        if(aFill && aFill->isUsed())
        {
            // if yes, fill the local SvxBrushItem using the new fill attributes
            // as good as possible to have an instance for the pointer to point
            // to and return as state that it is set
            static std::unique_ptr<SvxBrushItem> aSvxBrushItem; //(RES_BACKGROUND);

            aSvxBrushItem = getSvxBrushItemFromSourceSet(m_aSet, RES_BACKGROUND, bSrchInParent);
            if( ppItem )
                *ppItem = aSvxBrushItem.get();

            return SfxItemState::SET;
        }

        // if not, reset pointer and return SfxItemState::DEFAULT to signal that
        // the item is not set
        if( ppItem )
            *ppItem = nullptr;

        return SfxItemState::DEFAULT;
    }

    return m_aSet.GetItemState( nWhich, bSrchInParent, ppItem );
}

SfxItemState SwFormat::GetBackgroundState(std::unique_ptr<SvxBrushItem>& rItem) const
{
    if (supportsFullDrawingLayerFillAttributeSet())
    {
        // FALLBACKBREAKHERE should not be used; instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST]
        const drawinglayer::attribute::SdrAllFillAttributesHelperPtr aFill = getSdrAllFillAttributesHelper();

        // check if the new fill attributes are used
        if(aFill && aFill->isUsed())
        {
            // if yes, fill the local SvxBrushItem using the new fill attributes
            // as good as possible to have an instance for the pointer to point
            // to and return as state that it is set
            rItem = getSvxBrushItemFromSourceSet(m_aSet, RES_BACKGROUND);
            return SfxItemState::SET;
        }

        // if not return SfxItemState::DEFAULT to signal that the item is not set
        return SfxItemState::DEFAULT;
    }

    const SvxBrushItem* pItem = nullptr;
    SfxItemState eRet = m_aSet.GetItemState(RES_BACKGROUND, true, &pItem);
    if (pItem)
        rItem.reset(pItem->Clone());
    return eRet;
}

bool SwFormat::SetFormatAttr( const SfxPoolItem& rAttr )
{
    const sal_uInt16 nWhich = rAttr.Which();
    InvalidateInSwFntCache( nWhich );
    InvalidateInSwCache( nWhich );

    bool bRet = false;

    if (RES_BACKGROUND == rAttr.Which() && supportsFullDrawingLayerFillAttributeSet())
    {
        // FALLBACKBREAKHERE should not be used; instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST]
        SAL_INFO("sw.core", "Do no longer use SvxBrushItem, instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST] FillAttributes (simple fallback is in place and used)");
        SfxItemSetFixed<XATTR_FILL_FIRST, XATTR_FILL_LAST> aTempSet(*m_aSet.GetPool());
        const SvxBrushItem& rSource = rAttr.StaticWhichCast(RES_BACKGROUND);

        // fill a local ItemSet with the attributes corresponding as good as possible
        // to the new fill properties [XATTR_FILL_FIRST .. XATTR_FILL_LAST] and set these
        // as ItemSet
        setSvxBrushItemAsFillAttributesToTargetSet(rSource, aTempSet);

        if(IsModifyLocked())
        {
            bRet = m_aSet.Put( aTempSet );
            if( bRet )
            {
                m_aSet.SetModifyAtAttr( this );
            }
        }
        else
        {
            SwAttrSet aOld(*m_aSet.GetPool(), m_aSet.GetRanges()), aNew(*m_aSet.GetPool(), m_aSet.GetRanges());

            bRet = m_aSet.Put_BC(aTempSet, &aOld, &aNew);

            if(bRet)
            {
                m_aSet.SetModifyAtAttr(this);
                sw::ClientNotifyAttrChg(*this, m_aSet, aOld, aNew);
            }
        }

        return bRet;
    }

    // if Modify is locked then no modifications will be sent;
    // but call Modify always for FrameFormats
    const sal_uInt16 nFormatWhich = Which();
    if( IsModifyLocked() ||
        ( !HasWriterListeners() &&
          (RES_GRFFMTCOLL == nFormatWhich  ||
           RES_TXTFMTCOLL == nFormatWhich ) ) )
    {
        bRet = nullptr != m_aSet.Put( rAttr );
        if( bRet )
            m_aSet.SetModifyAtAttr( this );
        // #i71574#
        if ( nFormatWhich == RES_TXTFMTCOLL && rAttr.Which() == RES_PARATR_NUMRULE )
        {
            TextFormatCollFunc::CheckTextFormatCollForDeletionOfAssignmentToOutlineStyle( this );
        }
    }
    else
    {
        // copy only array with attributes delta
        SwAttrSet aOld( *m_aSet.GetPool(), m_aSet.GetRanges() ),
                  aNew( *m_aSet.GetPool(), m_aSet.GetRanges() );

        bRet = m_aSet.Put_BC( rAttr, &aOld, &aNew );
        if( bRet )
        {
            // some special treatments for attributes
            m_aSet.SetModifyAtAttr( this );

            sw::ClientNotifyAttrChg(*this, m_aSet, aOld, aNew);
        }
    }
    return bRet;
}

bool SwFormat::SetFormatAttr( const SfxItemSet& rSet )
{
    if( !rSet.Count() )
        return false;

    InvalidateInSwCache(RES_ATTRSET_CHG);
    InvalidateInSwFntCache(RES_ATTRSET_CHG);

    bool bRet = false;

    // Use local copy to be able to apply needed changes, e.g. call
    // CheckForUniqueItemForLineFillNameOrIndex which is needed for NameOrIndex stuff
    SfxItemSet aTempSet(rSet);

    // Need to check for unique item for DrawingLayer items of type NameOrIndex
    // and evtl. correct that item to ensure unique names for that type. This call may
    // modify/correct entries inside of the given SfxItemSet
    if(GetDoc())
    {
        GetDoc()->CheckForUniqueItemForLineFillNameOrIndex(aTempSet);
    }

    if (supportsFullDrawingLayerFillAttributeSet())
    {
        if(const SvxBrushItem* pSource = aTempSet.GetItemIfSet(RES_BACKGROUND, false))
        {
            // FALLBACKBREAKHERE should not be used; instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST]
            SAL_INFO("sw.core", "Do no longer use SvxBrushItem, instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST] FillAttributes (simple fallback is in place and used)");

            // copy all items to be set anyways to a local ItemSet with is also prepared for the new
            // fill attribute ranges [XATTR_FILL_FIRST .. XATTR_FILL_LAST]. Add the attributes
            // corresponding as good as possible to the new fill properties and set the whole ItemSet
            setSvxBrushItemAsFillAttributesToTargetSet(*pSource, aTempSet);

            if(IsModifyLocked())
            {
                bRet = m_aSet.Put( aTempSet );
                if( bRet )
                {
                    m_aSet.SetModifyAtAttr( this );
                }
            }
            else
            {
                SwAttrSet aOld(*m_aSet.GetPool(), m_aSet.GetRanges()), aNew(*m_aSet.GetPool(), m_aSet.GetRanges());

                bRet = m_aSet.Put_BC(aTempSet, &aOld, &aNew);

                if(bRet)
                {
                    m_aSet.SetModifyAtAttr(this);
                    sw::ClientNotifyAttrChg(*this, m_aSet, aOld, aNew);
                }
            }

            return bRet;
        }
    }

    // if Modify is locked then no modifications will be sent;
    // but call Modify always for FrameFormats
    const sal_uInt16 nFormatWhich = Which();
    if ( IsModifyLocked() ||
         ( !HasWriterListeners() &&
           ( RES_GRFFMTCOLL == nFormatWhich ||
             RES_TXTFMTCOLL == nFormatWhich ) ) )
    {
        bRet = m_aSet.Put( aTempSet );
        if( bRet )
            m_aSet.SetModifyAtAttr( this );
        // #i71574#
        if ( nFormatWhich == RES_TXTFMTCOLL )
        {
            TextFormatCollFunc::CheckTextFormatCollForDeletionOfAssignmentToOutlineStyle( this );
        }
    }
    else
    {
        SwAttrSet aOld( *m_aSet.GetPool(), m_aSet.GetRanges() ),
                  aNew( *m_aSet.GetPool(), m_aSet.GetRanges() );
        bRet = m_aSet.Put_BC( aTempSet, &aOld, &aNew );
        if( bRet )
        {
            // some special treatments for attributes
            m_aSet.SetModifyAtAttr( this );
            sw::ClientNotifyAttrChg(*this, m_aSet, aOld, aNew);
        }
    }
    return bRet;
}

// remove Hint using nWhich from array with delta
bool SwFormat::ResetFormatAttr( sal_uInt16 nWhich1, sal_uInt16 nWhich2 )
{
    if( !m_aSet.Count() )
        return false;

    if( !nWhich2 || nWhich2 < nWhich1 )
        nWhich2 = nWhich1; // then set to 1st ID, only this item

    for( sal_uInt16 n = nWhich1; n < nWhich2; ++n )
        InvalidateInSwFntCache( n );
    for( sal_uInt16 n = nWhich1; n < nWhich2 && IsInCache(); ++n )
        InvalidateInSwCache( n );

    // if Modify is locked then no modifications will be sent
    if( IsModifyLocked() )
        return 0 != (( nWhich2 == nWhich1 )
                     ? m_aSet.ClearItem( nWhich1 )
                     : m_aSet.ClearItem_BC( nWhich1, nWhich2 ));

    SwAttrSet aOld( *m_aSet.GetPool(), m_aSet.GetRanges() ),
              aNew( *m_aSet.GetPool(), m_aSet.GetRanges() );
    bool bRet = 0 != m_aSet.ClearItem_BC( nWhich1, nWhich2, &aOld, &aNew );
    if( bRet )
        sw::ClientNotifyAttrChg(*this, m_aSet, aOld, aNew);
    return bRet;
}

// #i73790#
sal_uInt16 SwFormat::ResetAllFormatAttr()
{
    if( !m_aSet.Count() )
        return 0;

    InvalidateInSwCache(RES_ATTRSET_CHG);
    InvalidateInSwFntCache(RES_ATTRSET_CHG);

    // if Modify is locked then no modifications will be sent
    if( IsModifyLocked() )
        return m_aSet.ClearItem();

    SwAttrSet aOld( *m_aSet.GetPool(), m_aSet.GetRanges() ),
              aNew( *m_aSet.GetPool(), m_aSet.GetRanges() );
    bool bRet = 0 != m_aSet.ClearItem_BC( 0, &aOld, &aNew );
    if( bRet )
        sw::ClientNotifyAttrChg(*this, m_aSet, aOld, aNew);
    return aNew.Count();
}

void SwFormat::DelDiffs( const SfxItemSet& rSet )
{
    if( !m_aSet.Count() )
        return;

    InvalidateInSwCache(RES_ATTRSET_CHG);
    InvalidateInSwFntCache(RES_ATTRSET_CHG);

    // if Modify is locked then no modifications will be sent
    if( IsModifyLocked() )
    {
        m_aSet.Intersect( rSet );
        return;
    }

    SwAttrSet aOld( *m_aSet.GetPool(), m_aSet.GetRanges() ),
              aNew( *m_aSet.GetPool(), m_aSet.GetRanges() );
    bool bRet = 0 != m_aSet.Intersect_BC( rSet, &aOld, &aNew );
    if( bRet )
        sw::ClientNotifyAttrChg(*this, m_aSet, aOld, aNew);
}

void SwFormat::SetPageFormatToDefault()
{
    const sal_Int32 nSize = o3tl::convert(2, o3tl::Length::cm, o3tl::Length::twip);
    SetFormatAttr(SvxLRSpaceItem(nSize, nSize, nSize, 0, RES_LR_SPACE));
    SetFormatAttr(SvxULSpaceItem(nSize, nSize, RES_UL_SPACE));
}

/** SwFormat::IsBackgroundTransparent

    Virtual method to determine, if background of format is transparent.
    Default implementation returns false. Thus, subclasses have to override
    method, if the specific subclass can have a transparent background.

    @return false, default implementation
*/
bool SwFormat::IsBackgroundTransparent() const
{
    return false;
}

/*
 * Document Interface Access
 */
const IDocumentSettingAccess& SwFormat::getIDocumentSettingAccess() const { return GetDoc()->GetDocumentSettingManager(); }
const IDocumentDrawModelAccess& SwFormat::getIDocumentDrawModelAccess() const { return GetDoc()->getIDocumentDrawModelAccess(); }
IDocumentDrawModelAccess& SwFormat::getIDocumentDrawModelAccess() { return GetDoc()->getIDocumentDrawModelAccess(); }
const IDocumentLayoutAccess& SwFormat::getIDocumentLayoutAccess() const { return GetDoc()->getIDocumentLayoutAccess(); }
IDocumentLayoutAccess& SwFormat::getIDocumentLayoutAccess() { return GetDoc()->getIDocumentLayoutAccess(); }
IDocumentTimerAccess& SwFormat::getIDocumentTimerAccess() { return GetDoc()->getIDocumentTimerAccess(); }
IDocumentFieldsAccess& SwFormat::getIDocumentFieldsAccess() { return GetDoc()->getIDocumentFieldsAccess(); }
IDocumentChartDataProviderAccess& SwFormat::getIDocumentChartDataProviderAccess() { return GetDoc()->getIDocumentChartDataProviderAccess(); }

void SwFormat::GetGrabBagItem(uno::Any& rVal) const
{
    if (m_pGrabBagItem)
        m_pGrabBagItem->QueryValue(rVal);
    else
        rVal <<= uno::Sequence<beans::PropertyValue>();
}

void SwFormat::SetGrabBagItem(const uno::Any& rVal)
{
    if (!m_pGrabBagItem)
        m_pGrabBagItem = std::make_shared<SfxGrabBagItem>();

    m_pGrabBagItem->PutValue(rVal, 0);
}

std::unique_ptr<SvxBrushItem> SwFormat::makeBackgroundBrushItem(bool bInP) const
{
    if (supportsFullDrawingLayerFillAttributeSet())
    {
        // FALLBACKBREAKHERE should not be used; instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST]
        SAL_INFO("sw.core", "Do no longer use SvxBrushItem, instead use [XATTR_FILL_FIRST .. XATTR_FILL_LAST] FillAttributes (simple fallback is in place and used)");

        // fill the local static SvxBrushItem from the current ItemSet so that
        // the fill attributes [XATTR_FILL_FIRST .. XATTR_FILL_LAST] are used
        // as good as possible to create a fallback representation and return that
        return getSvxBrushItemFromSourceSet(m_aSet, RES_BACKGROUND, bInP);
    }

    return std::unique_ptr<SvxBrushItem>(m_aSet.GetBackground(bInP).Clone());
}

drawinglayer::attribute::SdrAllFillAttributesHelperPtr SwFormat::getSdrAllFillAttributesHelper() const
{
    return drawinglayer::attribute::SdrAllFillAttributesHelperPtr();
}

void SwFormat::RemoveAllUnos()
{
    SwPtrMsgPoolItem aMsgHint(RES_REMOVE_UNO_OBJECT, this);
    SwClientNotify(*this, sw::LegacyModifyHint(&aMsgHint, &aMsgHint));
}

SwFormatsBase::~SwFormatsBase()
{}

SwFormat* SwFormatsBase::FindFormatByName( const OUString& rName ) const
{
    SwFormat* pFnd = nullptr;
    for( size_t n = 0; n < GetFormatCount(); ++n )
    {
        // Does the Doc already contain the template?
        if( GetFormat(n)->HasName( rName ) )
        {
            pFnd = GetFormat(n);
            break;
        }
    }
    return pFnd;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
