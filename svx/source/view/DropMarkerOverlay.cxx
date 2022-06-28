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

#include <svx/sdr/view/DropMarkerOverlay.hxx>
#include <svx/svdobj.hxx>
#include <svx/sdr/overlay/overlaypolypolygon.hxx>
#include <svx/sdrpaintwindow.hxx>

// helper class for all D&D overlays

void SdrDropMarkerOverlay::ImplCreateOverlays(const SdrView& rView,
                                              const basegfx::B2DPolyPolygon& rLinePolyPolygon)
{
    for (sal_uInt32 a(0); a < rView.PaintWindowCount(); a++)
    {
        SdrPaintWindow* pCandidate = rView.GetPaintWindow(a);
        const rtl::Reference<sdr::overlay::OverlayManager>& xTargetOverlay
            = pCandidate->GetOverlayManager();

        if (xTargetOverlay.is())
        {
            std::unique_ptr<sdr::overlay::OverlayPolyPolygonStripedAndFilled> pNew(
                new sdr::overlay::OverlayPolyPolygonStripedAndFilled(rLinePolyPolygon));

            xTargetOverlay->add(*pNew);
            maObjects.append(std::move(pNew));
        }
    }
}

SdrDropMarkerOverlay::SdrDropMarkerOverlay(const SdrView& rView, const SdrObject& rObject)
{
    ImplCreateOverlays(rView, rObject.TakeXorPoly());
}

SdrDropMarkerOverlay::SdrDropMarkerOverlay(const SdrView& rView, const tools::Rectangle& rRectangle)
{
    basegfx::B2DPolygon aB2DPolygon;

    aB2DPolygon.append(basegfx::B2DPoint(rRectangle.Left(), rRectangle.Top()));
    aB2DPolygon.append(basegfx::B2DPoint(rRectangle.Right(), rRectangle.Top()));
    aB2DPolygon.append(basegfx::B2DPoint(rRectangle.Right(), rRectangle.Bottom()));
    aB2DPolygon.append(basegfx::B2DPoint(rRectangle.Left(), rRectangle.Bottom()));
    aB2DPolygon.setClosed(true);

    ImplCreateOverlays(rView, basegfx::B2DPolyPolygon(aB2DPolygon));
}

SdrDropMarkerOverlay::SdrDropMarkerOverlay(const SdrView& rView, const Point& rStart,
                                           const Point& rEnd)
{
    basegfx::B2DPolygon aB2DPolygon;

    aB2DPolygon.append(basegfx::B2DPoint(rStart.X(), rStart.Y()));
    aB2DPolygon.append(basegfx::B2DPoint(rEnd.X(), rEnd.Y()));
    aB2DPolygon.setClosed(true);

    ImplCreateOverlays(rView, basegfx::B2DPolyPolygon(aB2DPolygon));
}

SdrDropMarkerOverlay::~SdrDropMarkerOverlay()
{
    // The OverlayObjects are cleared using the destructor of OverlayObjectList.
    // That destructor calls clear() at the list which removes all objects from the
    // OverlayManager and deletes them.
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
