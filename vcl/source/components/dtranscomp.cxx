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
#include <sal/log.hxx>

#include <comphelper/lok.hxx>
#include <osl/mutex.hxx>
#include <tools/debug.hxx>
#include <vcl/svapp.hxx>

#include <dndlistenercontainer.hxx>
#include <factory.hxx>
#include <svdata.hxx>
#include <salinst.hxx>

#include <com/sun/star/lang/IllegalArgumentException.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/lang/XSingleServiceFactory.hpp>
#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/datatransfer/XTransferable.hpp>
#include <com/sun/star/datatransfer/clipboard/XClipboardListener.hpp>
#include <com/sun/star/datatransfer/clipboard/XSystemClipboard.hpp>
#include <com/sun/star/datatransfer/dnd/XDragSource.hpp>
#include <com/sun/star/datatransfer/dnd/XDropTarget.hpp>
#include <com/sun/star/datatransfer/dnd/DNDConstants.hpp>

#include <cppuhelper/compbase.hxx>
#include <cppuhelper/supportsservice.hxx>

using namespace com::sun::star;
using namespace com::sun::star::uno;
using namespace com::sun::star::lang;

namespace vcl
{
namespace {
class GenericDropTarget : public cppu::WeakComponentImplHelper<
                                           datatransfer::dnd::XDropTarget,
                                           XInitialization,
                                           css::lang::XServiceInfo
                                           >
{
    osl::Mutex m_aMutex;
    ::std::vector< css::uno::Reference< css::datatransfer::dnd::XDropTargetListener > > m_aListeners;
public:
    GenericDropTarget() : WeakComponentImplHelper( m_aMutex )
    {}

    // XInitialization
    virtual void        SAL_CALL initialize( const Sequence< Any >& args ) override;

    // XDropTarget
    virtual void        SAL_CALL addDropTargetListener( const Reference< css::datatransfer::dnd::XDropTargetListener >& ) override;
    virtual void        SAL_CALL removeDropTargetListener( const Reference< css::datatransfer::dnd::XDropTargetListener >& ) override;
    virtual sal_Bool    SAL_CALL isActive() override;
    virtual void        SAL_CALL setActive( sal_Bool active ) override;
    virtual sal_Int8    SAL_CALL getDefaultActions() override;
    virtual void        SAL_CALL setDefaultActions( sal_Int8 actions ) override;

    void dragEnter( const com::sun::star::datatransfer::dnd::DropTargetDragEnterEvent& dtde ) noexcept
    {
        osl::ClearableGuard< ::osl::Mutex > aGuard( m_aMutex );
        std::vector< Reference< com::sun::star::datatransfer::dnd::XDropTargetListener > > aListeners( m_aListeners );
        aGuard.clear();

        for (auto const& listener : aListeners)
        {
            listener->dragEnter(dtde);
        }
    }

    OUString SAL_CALL getImplementationName() override
    { return "com.sun.star.datatransfer.dnd.VclGenericDropTarget"; }

    sal_Bool SAL_CALL supportsService(OUString const & ServiceName) override
    { return cppu::supportsService(this, ServiceName); }

    css::uno::Sequence<OUString> SAL_CALL getSupportedServiceNames() override
    { return getSupportedServiceNames_static(); }

    static Sequence< OUString > getSupportedServiceNames_static()
    {
      return { "com.sun.star.datatransfer.dnd.GenericDropTarget" };
    }
};

/*
*   generic DragSource dummy
*/
class GenericDragSource : public cppu::WeakComponentImplHelper<
            datatransfer::dnd::XDragSource,
            XInitialization,
            css::lang::XServiceInfo
            >
{
    osl::Mutex                          m_aMutex;
    css::uno::Reference<css::datatransfer::XTransferable> m_xTrans;
    css::uno::Reference<css::datatransfer::dnd::XDragSourceListener> m_xListener;
    std::vector<GenericDropTarget*> m_pTarget;
public:
    GenericDragSource() : WeakComponentImplHelper( m_aMutex ) {}

    static GenericDragSource& get( const OUString& rDisplayName );

    void registerDropTarget( /*::Window aXLIB_Window,*/ GenericDropTarget* pTarget );

    void accept( sal_Int8 dragOperation );

    // XDragSource
    virtual sal_Bool    SAL_CALL isDragImageSupported() override;
    virtual sal_Int32   SAL_CALL getDefaultCursor( sal_Int8 dragAction ) override;
    virtual void        SAL_CALL startDrag(
                                     const datatransfer::dnd::DragGestureEvent& trigger,
                                     sal_Int8 sourceActions, sal_Int32 cursor, sal_Int32 image,
                                     const Reference< datatransfer::XTransferable >& transferable,
                                     const Reference< datatransfer::dnd::XDragSourceListener >& listener
                                     ) override;

    // XInitialization
    virtual void        SAL_CALL initialize( const Sequence< Any >& arguments ) override;

    OUString SAL_CALL getImplementationName() override
    { return "com.sun.star.datatransfer.dnd.VclGenericDragSource"; }

    sal_Bool SAL_CALL supportsService(OUString const & ServiceName) override
    { return cppu::supportsService(this, ServiceName); }

    css::uno::Sequence<OUString> SAL_CALL getSupportedServiceNames() override
    { return getSupportedServiceNames_static(); }

    static Sequence< OUString > getSupportedServiceNames_static()
    {
       return { "com.sun.star.datatransfer.dnd.GenericDragSource" };
    }

private:
    static std::unordered_map< OUString, GenericDragSource* >& getInstances();
};

class GenericDropTargetDragContext :
    public ::cppu::WeakImplHelper<css::datatransfer::dnd::XDropTargetDragContext>
{
    GenericDragSource* m_pDragSource;

public:
    GenericDropTargetDragContext(GenericDragSource* pDragSource);

    // XDropTargetDragContext
    virtual void SAL_CALL acceptDrag( sal_Int8 dragOperation ) override;
    virtual void SAL_CALL rejectDrag() override;
};

// generic implementation to satisfy SalInstance
class GenericClipboard :
        public cppu::WeakComponentImplHelper<
        datatransfer::clipboard::XSystemClipboard,
        XServiceInfo
        >
{
    osl::Mutex                                                              m_aMutex;
    Reference< css::datatransfer::XTransferable >                           m_aContents;
    Reference< css::datatransfer::clipboard::XClipboardOwner >              m_aOwner;
    std::vector< Reference< css::datatransfer::clipboard::XClipboardListener > > m_aListeners;

public:

    GenericClipboard() : cppu::WeakComponentImplHelper<
        datatransfer::clipboard::XSystemClipboard,
        XServiceInfo
        >( m_aMutex )
    {}

    /*
     * XServiceInfo
     */

    virtual OUString SAL_CALL getImplementationName() override;
    virtual sal_Bool SAL_CALL supportsService( const OUString& ServiceName ) override;
    virtual Sequence< OUString > SAL_CALL getSupportedServiceNames() override;

    static Sequence< OUString > getSupportedServiceNames_static();

    /*
     * XClipboard
     */

    virtual Reference< css::datatransfer::XTransferable > SAL_CALL getContents() override;

    virtual void SAL_CALL setContents(
        const Reference< css::datatransfer::XTransferable >& xTrans,
        const Reference< css::datatransfer::clipboard::XClipboardOwner >& xClipboardOwner ) override;

    virtual OUString SAL_CALL getName() override;

    /*
     * XClipboardEx
     */

    virtual sal_Int8 SAL_CALL getRenderingCapabilities() override;

    /*
     * XClipboardNotifier
     */
    virtual void SAL_CALL addClipboardListener(
        const Reference< css::datatransfer::clipboard::XClipboardListener >& listener ) override;

    virtual void SAL_CALL removeClipboardListener(
        const Reference< css::datatransfer::clipboard::XClipboardListener >& listener ) override;
};

}

GenericDropTargetDragContext::GenericDropTargetDragContext(GenericDragSource* pDragSource)
: m_pDragSource(pDragSource)
{
}

void GenericDropTargetDragContext::acceptDrag( sal_Int8 dragOperation )
{
    m_pDragSource->accept( dragOperation );
}

void GenericDropTargetDragContext::rejectDrag()
{
}

Sequence< OUString > GenericClipboard::getSupportedServiceNames_static()
{
    Sequence< OUString > aRet { "com.sun.star.datatransfer.clipboard.SystemClipboard" };
    return aRet;
}

OUString GenericClipboard::getImplementationName()
{
    return "com.sun.star.datatransfer.VCLGenericClipboard";
}

Sequence< OUString > GenericClipboard::getSupportedServiceNames()
{
    return getSupportedServiceNames_static();
}

sal_Bool GenericClipboard::supportsService( const OUString& ServiceName )
{
    return cppu::supportsService(this, ServiceName);
}

Reference< css::datatransfer::XTransferable > GenericClipboard::getContents()
{
    return m_aContents;
}

void GenericClipboard::setContents(
        const Reference< css::datatransfer::XTransferable >& xTrans,
        const Reference< css::datatransfer::clipboard::XClipboardOwner >& xClipboardOwner )
{
    osl::ClearableMutexGuard aGuard( m_aMutex );
    Reference< datatransfer::clipboard::XClipboardOwner > xOldOwner( m_aOwner );
    Reference< datatransfer::XTransferable > xOldContents( m_aContents );
    m_aContents = xTrans;
    m_aOwner = xClipboardOwner;

    std::vector< Reference< datatransfer::clipboard::XClipboardListener > > aListeners( m_aListeners );
    datatransfer::clipboard::ClipboardEvent aEv;
    aEv.Contents = m_aContents;

    aGuard.clear();

    if( xOldOwner.is() && xOldOwner != xClipboardOwner )
        xOldOwner->lostOwnership( this, xOldContents );
    for (auto const& listener : aListeners)
    {
        listener->changedContents( aEv );
    }
}

OUString GenericClipboard::getName()
{
    return "CLIPBOARD";
}

sal_Int8 GenericClipboard::getRenderingCapabilities()
{
    return 0;
}

void GenericClipboard::addClipboardListener( const Reference< datatransfer::clipboard::XClipboardListener >& listener )
{
    osl::MutexGuard aGuard(m_aMutex);

    m_aListeners.push_back( listener );
}

void GenericClipboard::removeClipboardListener( const Reference< datatransfer::clipboard::XClipboardListener >& listener )
{
    osl::MutexGuard aGuard(m_aMutex);

    m_aListeners.erase(std::remove(m_aListeners.begin(), m_aListeners.end(), listener), m_aListeners.end());
}



extern "C" SAL_DLLPUBLIC_EXPORT css::uno::XInterface*
vcl_SystemClipboard_get_implementation(
    css::uno::XComponentContext* , css::uno::Sequence<css::uno::Any> const& args)
{
    SolarMutexGuard aGuard;
    auto xClipboard = ImplGetSVData()->mpDefInst->CreateClipboard( args );
    if (xClipboard.is())
        xClipboard->acquire();
    return xClipboard.get();
}

std::unordered_map< OUString, GenericDragSource* >& GenericDragSource::getInstances()
{
    static std::unordered_map< OUString, GenericDragSource* > aInstances;
    return aInstances;
}

GenericDragSource& GenericDragSource::get( const OUString& rDisplayName )
{
    osl::MutexGuard aGuard( *osl::Mutex::getGlobalMutex() );

    OUString aDisplayName( rDisplayName );
    if( aDisplayName.isEmpty() )
        if (auto const env = getenv( "DISPLAY" )) {
            aDisplayName = OStringToOUString( env, RTL_TEXTENCODING_ISO_8859_1 );
        }
    GenericDragSource* pInstance = nullptr;

    std::unordered_map< OUString, GenericDragSource* >::iterator it = GenericDragSource::getInstances().find( aDisplayName );
    if( it != GenericDragSource::getInstances().end() )
        pInstance = it->second;
    else pInstance = GenericDragSource::getInstances()[ aDisplayName ] = new GenericDragSource();

    return *pInstance;
}

void GenericDragSource::registerDropTarget( /*::Window aXLIB_Window,*/ GenericDropTarget* pTarget )
{
    m_pTarget.push_back(pTarget);
}

sal_Bool GenericDragSource::isDragImageSupported()
{
    return false;
}

sal_Int32 GenericDragSource::getDefaultCursor( sal_Int8 )
{
    return 0;
}

void GenericDragSource::accept( sal_Int8 dragOperation )
{
    if( m_xListener.is() )
    {
        sal_Int8 nNewDragAction;
        if( dragOperation & datatransfer::dnd::DNDConstants::ACTION_MOVE )
            nNewDragAction = datatransfer::dnd::DNDConstants::ACTION_MOVE;
        else if( dragOperation & datatransfer::dnd::DNDConstants::ACTION_COPY )
            nNewDragAction = datatransfer::dnd::DNDConstants::ACTION_COPY;
        else if( dragOperation & datatransfer::dnd::DNDConstants::ACTION_LINK )
            nNewDragAction = datatransfer::dnd::DNDConstants::ACTION_LINK;
        else
            nNewDragAction = datatransfer::dnd::DNDConstants::ACTION_NONE;
        //nNewDragAction &= m_nSourceActions;

        // if( nNewDragAction != m_nTargetAcceptAction )
        // {
        //     setCursor( getDefaultCursor( nNewDragAction ), m_aDropWindow );
        //     m_nTargetAcceptAction = nNewDragAction;
        // }

        com::sun::star::datatransfer::dnd::DragSourceDragEvent dsde;
        dsde.Source             = static_cast< OWeakObject* >(this);
        //dsde.DragSourceContext  = new DragSourceContext( m_aDropWindow, *this );
        dsde.DragSource         = static_cast< XDragSource* >(this);
        dsde.DropAction         = datatransfer::dnd::DNDConstants::ACTION_NONE;//m_nSourceActions;
        dsde.UserAction         = datatransfer::dnd::DNDConstants::ACTION_NONE;//getUserDragAction();

        css::uno::Reference< com::sun::star::datatransfer::dnd::XDragSourceListener > xListener( m_xListener );
        // caution: do not change anything after this
        if( xListener.is() )
            xListener->dragOver( dsde );
    }
}

void GenericDragSource::startDrag( const datatransfer::dnd::DragGestureEvent& event,
                                   sal_Int8 /*sourceActions*/, sal_Int32 /*cursor*/, sal_Int32 /*image*/,
                                   const Reference< datatransfer::XTransferable >& rTrans,
                                   const Reference< datatransfer::dnd::XDragSourceListener >& listener
                                   )
{
    if (comphelper::LibreOfficeKit::isActive()) {
        m_xTrans = rTrans;
        m_xListener = listener;

        // m_pTarget
        // updateDragWindow( root_x, root_y, aRoot );

        for(auto pTarget : m_pTarget)
        {
            com::sun::star::datatransfer::dnd::DropTargetDragEnterEvent dtde;
            dtde.Source                 = static_cast< OWeakObject* >( pTarget );
            dtde.Context                = new GenericDropTargetDragContext( this );
            dtde.LocationX              = event.DragOriginX;
            dtde.LocationY              = event.DragOriginY;
            dtde.DropAction             = datatransfer::dnd::DNDConstants::ACTION_COPY; //??
            dtde.SourceActions          = datatransfer::dnd::DNDConstants::ACTION_COPY;
            dtde.SupportedDataFlavors   = rTrans->getTransferDataFlavors();
            pTarget->dragEnter( dtde );
        }

        return;
    }

    datatransfer::dnd::DragSourceDropEvent aEv;
    aEv.DropAction = datatransfer::dnd::DNDConstants::ACTION_COPY;
    aEv.DropSuccess = false;
    listener->dragDropEnd( aEv );
}

void GenericDragSource::initialize( const Sequence< Any >& arguments )
{
    OUString aDisplayName;

    if( arguments.hasElements() )
    {
        css::uno::Reference< css::awt::XDisplayConnection > xConn;
        arguments.getConstArray()[0] >>= xConn;
        if( xConn.is() )
        {
            Any aIdentifier;
            aIdentifier >>= aDisplayName;
        }
    }

    getInstances()[aDisplayName] = this;
}

Sequence< OUString > DragSource_getSupportedServiceNames()
{
#if defined MACOSX
    return { "com.sun.star.datatransfer.dnd.OleDragSource" };
#elif defined UNX
    return { "com.sun.star.datatransfer.dnd.X11DragSource" };
#else
    return { "com.sun.star.datatransfer.dnd.VclGenericDragSource" };
#endif
}

OUString DragSource_getImplementationName()
{
#if defined MACOSX
    return "com.sun.star.comp.datatransfer.dnd.OleDragSource_V1";
#elif defined UNX
    return "com.sun.star.datatransfer.dnd.XdndSupport";
#else
    return "com.sun.star.datatransfer.dnd.VclGenericDragSource";
#endif
}

Reference< XInterface > DragSource_createInstance( const Reference< XMultiServiceFactory >&  )
{
    SolarMutexGuard aGuard;
    Reference< XInterface > xResult = ImplGetSVData()->mpDefInst->CreateDragSource();
    return xResult;
}

/*
*   generic DragSource dummy
*/

void GenericDropTarget::initialize( const Sequence< Any >& arguments )
{
    if( arguments.getLength() <= 1 )
        return;

    OUString aDisplayName;
    Reference< css::awt::XDisplayConnection > xConn;
    arguments.getConstArray()[0] >>= xConn;
    if( xConn.is() )
    {
        Any aIdentifier;
        aIdentifier >>= aDisplayName;
        SAL_DEBUG("DIS NAME DROP TARGET: " << aDisplayName);
    }

    GenericDragSource::get(aDisplayName).registerDropTarget(this);
}

void GenericDropTarget::addDropTargetListener( const Reference< css::datatransfer::dnd::XDropTargetListener >& listener)
{
    m_aListeners.push_back(listener);
}

void GenericDropTarget::removeDropTargetListener( const Reference< css::datatransfer::dnd::XDropTargetListener >& )
{
}

sal_Bool GenericDropTarget::isActive()
{
    return true;
}

void GenericDropTarget::setActive( sal_Bool )
{
}

sal_Int8 GenericDropTarget::getDefaultActions()
{
    return 0;
}

void GenericDropTarget::setDefaultActions( sal_Int8)
{
}

Sequence< OUString > DropTarget_getSupportedServiceNames()
{
#if defined MACOSX
    return {  "com.sun.star.datatransfer.dnd.OleDropTarget" };
#elif defined UNX
    return { "com.sun.star.datatransfer.dnd.X11DropTarget" };
#else
    return GenericDropTarget::getSupportedServiceNames_static();
#endif
}

OUString DropTarget_getImplementationName()
{
    return
    #if defined MACOSX
    "com.sun.star.comp.datatransfer.dnd.OleDropTarget_V1"
    #elif defined UNX
    "com.sun.star.datatransfer.dnd.XdndDropTarget"
    #else
    "com.sun.star.datatransfer.dnd.VclGenericDropTarget"
    #endif
                   ;
}

Reference< XInterface > DropTarget_createInstance( const Reference< XMultiServiceFactory >&  )
{
    SolarMutexGuard aGuard;
    Reference< XInterface > xResult = ImplGetSVData()->mpDefInst->CreateDropTarget();
    return xResult;
}

} // namespace vcl

/*
*   SalInstance generic
*/
Reference< XInterface > SalInstance::CreateClipboard( const Sequence< Any >& arguments )
{
    if (arguments.hasElements()) {
        throw css::lang::IllegalArgumentException(
            "non-empty SalInstance::CreateClipboard arguments", {}, -1);
    }
    if (comphelper::LibreOfficeKit::isActive()) {
        // In LOK, each document view shall have its own clipboard instance, and the way that
        // (happens to?) work is that apparently this function is called at most once for each such
        // document view, so it is OK if we hand out a fresh instance on each call in LOK (whereas
        // in non-LOK below we keep handing out one single instance; see also
        // <https://lists.freedesktop.org/archives/libreoffice/2020-April/084824.html> "Re: Linux
        // SAL_USE_VCLPLUGIN=svp and the clipboard"):
        return Reference< XInterface >( static_cast<cppu::OWeakObject *>(new vcl::GenericClipboard()) );
    }
    DBG_TESTSOLARMUTEX();
    if (!m_clipboard.is()) {
        m_clipboard = static_cast<cppu::OWeakObject *>(new vcl::GenericClipboard());
    }
    return m_clipboard;
}

Reference< XInterface > SalInstance::CreateDragSource()
{
    return Reference< XInterface >( static_cast<cppu::OWeakObject *>(new vcl::GenericDragSource()) );
}

Reference< XInterface > SalInstance::CreateDropTarget()
{
    return Reference< XInterface >( static_cast<cppu::OWeakObject *>(new vcl::GenericDropTarget()) );
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
