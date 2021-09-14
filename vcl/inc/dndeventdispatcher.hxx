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

#ifndef INCLUDED_VCL_INC_DNDEVENTDISPATCHER_HXX
#define INCLUDED_VCL_INC_DNDEVENTDISPATCHER_HXX

#include <com/sun/star/datatransfer/dnd/XDropTargetListener.hpp>
#include <com/sun/star/datatransfer/dnd/XDropTargetDragContext.hpp>
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

#include <com/sun/star/datatransfer/dnd/XDragGestureListener.hpp>
#include <cppuhelper/implbase.hxx>
#include <vcl/window.hxx>
#include <cppuhelper/compbase.hxx>
#include <cppuhelper/supportsservice.hxx>

using namespace com::sun::star;
using namespace com::sun::star::uno;
using namespace com::sun::star::lang;

namespace vcl
{
class GenericDropTarget : public cppu::WeakComponentImplHelper<
                                           css::datatransfer::dnd::XDropTarget,
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
    virtual void        SAL_CALL addDropTargetListener( const css::uno::Reference< css::datatransfer::dnd::XDropTargetListener >& ) override;
    virtual void        SAL_CALL removeDropTargetListener( const css::uno::Reference< css::datatransfer::dnd::XDropTargetListener >& ) override;
    virtual sal_Bool    SAL_CALL isActive() override;
    virtual void        SAL_CALL setActive( sal_Bool active ) override;
    virtual sal_Int8    SAL_CALL getDefaultActions() override;
    virtual void        SAL_CALL setDefaultActions( sal_Int8 actions ) override;

    void dragEnter( const com::sun::star::datatransfer::dnd::DropTargetDragEnterEvent& dtde ) noexcept
    {
        osl::ClearableGuard< ::osl::Mutex > aGuard( m_aMutex );
        std::vector< css::uno::Reference< com::sun::star::datatransfer::dnd::XDropTargetListener > > aListeners( m_aListeners );
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
                                     const css::uno::Reference< datatransfer::XTransferable >& transferable,
                                     const css::uno::Reference< datatransfer::dnd::XDragSourceListener >& listener
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

class GenericDropTargetDropContext :
    public ::cppu::WeakImplHelper<css::datatransfer::dnd::XDropTargetDropContext>
{
public:
    GenericDropTargetDropContext();
    // XDropTargetDropContext
    virtual void SAL_CALL acceptDrop( sal_Int8 dragOperation ) override;
    virtual void SAL_CALL rejectDrop() override;
    virtual void SAL_CALL dropComplete( sal_Bool success ) override;
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
}
class DNDEventDispatcher final : public ::cppu::WeakImplHelper<
    css::datatransfer::dnd::XDropTargetListener,
    css::datatransfer::dnd::XDropTargetDragContext,
    css::datatransfer::dnd::XDragGestureListener >
{
    VclPtr<vcl::Window> m_pTopWindow;

    VclPtr<vcl::Window> m_pCurrentWindow;
    void designate_currentwindow(vcl::Window *pWindow);
    DECL_LINK(WindowEventListener, VclWindowEvent&, void);

    ::osl::Mutex m_aMutex;
    css::uno::Sequence< css::datatransfer::DataFlavor > m_aDataFlavorList;

    vcl::Window* findTopLevelWindow(Point location);
    /*
     * fire the events on the dnd listener container of the specified window
     */

    /// @throws css::uno::RuntimeException
    static sal_Int32 fireDragEnterEvent( vcl::Window *pWindow, const css::uno::Reference< css::datatransfer::dnd::XDropTargetDragContext >& xContext,
        const sal_Int8 nDropAction, const Point& rLocation, const sal_Int8 nSourceAction,
        const css::uno::Sequence< css::datatransfer::DataFlavor >& aFlavorList );

    /// @throws css::uno::RuntimeException
    static sal_Int32 fireDragOverEvent( vcl::Window *pWindow, const css::uno::Reference< css::datatransfer::dnd::XDropTargetDragContext >& xContext,
        const sal_Int8 nDropAction, const Point& rLocation, const sal_Int8 nSourceAction );

    /// @throws css::uno::RuntimeException
    static sal_Int32 fireDragExitEvent( vcl::Window *pWindow );

    /// @throws css::uno::RuntimeException
    static sal_Int32 fireDropActionChangedEvent( vcl::Window *pWindow, const css::uno::Reference< css::datatransfer::dnd::XDropTargetDragContext >& xContext,
        const sal_Int8 nDropAction, const Point& rLocation, const sal_Int8 nSourceAction );

    /// @throws css::uno::RuntimeException
    static sal_Int32 fireDropEvent( vcl::Window *pWindow, const css::uno::Reference< css::datatransfer::dnd::XDropTargetDropContext >& xContext,
        const sal_Int8 nDropAction, const Point& rLocation, const sal_Int8 nSourceAction,
        const css::uno::Reference< css::datatransfer::XTransferable >& xTransferable );

    /// @throws css::uno::RuntimeException
    static sal_Int32 fireDragGestureEvent( vcl::Window *pWindow, const css::uno::Reference< css::datatransfer::dnd::XDragSource >& xSource,
        const css::uno::Any& event, const Point& rOrigin, const sal_Int8 nDragAction );

public:

    DNDEventDispatcher( vcl::Window * pTopWindow );
    virtual ~DNDEventDispatcher() override;

    /*
     * XDropTargetDragContext
     */

    virtual void SAL_CALL acceptDrag( sal_Int8 dropAction ) override;
    virtual void SAL_CALL rejectDrag() override;

    /*
     * XDropTargetListener
     */

    virtual void SAL_CALL drop( const css::datatransfer::dnd::DropTargetDropEvent& dtde ) override;
    virtual void SAL_CALL dragEnter( const css::datatransfer::dnd::DropTargetDragEnterEvent& dtdee ) override;
    virtual void SAL_CALL dragExit( const css::datatransfer::dnd::DropTargetEvent& dte ) override;
    virtual void SAL_CALL dragOver( const css::datatransfer::dnd::DropTargetDragEvent& dtde ) override;
    virtual void SAL_CALL dropActionChanged( const css::datatransfer::dnd::DropTargetDragEvent& dtde ) override;

    /*
     * XDragGestureListener
     */

    virtual void SAL_CALL dragGestureRecognized( const css::datatransfer::dnd::DragGestureEvent& dge ) override;

    /*
     * XEventListener
     */

    virtual void SAL_CALL disposing( const css::lang::EventObject& eo ) override;
};

#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
