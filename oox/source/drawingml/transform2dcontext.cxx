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

#include <drawingml/transform2dcontext.hxx>
#include <oox/helper/attributelist.hxx>
#include <oox/drawingml/shape.hxx>
#include <drawingml/customshapeproperties.hxx>
#include <drawingml/textbody.hxx>
#include <oox/token/namespaces.hxx>

using namespace ::com::sun::star;
using ::oox::core::ContextHandlerRef;

namespace oox::drawingml {

/** context to import a CT_Transform2D */
Transform2DContext::Transform2DContext( ContextHandler2Helper const & rParent, const AttributeList& rAttribs, Shape& rShape, bool btxXfrm ) noexcept
: ContextHandler2( rParent )
, mrShape( rShape )
, mbtxXfrm ( btxXfrm )
{
    if( !btxXfrm )
    {
        mrShape.setRotation( rAttribs.getInteger( XML_rot, 0 ) ); // 60000ths of a degree Positive angles are clockwise; negative angles are counter-clockwise
        mrShape.setFlip( rAttribs.getBool( XML_flipH, false ), rAttribs.getBool( XML_flipV, false ) );
    }
    else
    {
        if( rAttribs.hasAttribute( XML_rot ) )
            mrShape.getTextBody()->getTextProperties().moRotation = rAttribs.getInteger( XML_rot, 0 );
    }
}

ContextHandlerRef Transform2DContext::onCreateContext( sal_Int32 aElementToken, const AttributeList& rAttribs )
{
    if( mbtxXfrm )
    {
        // Workaround: only for rectangles
        const sal_Int32 nType = mrShape.getCustomShapeProperties()->getShapePresetType();
        if( nType == XML_rect || nType == XML_roundRect || nType == XML_ellipse )
        {
            switch( aElementToken )
            {
                case A_TOKEN( off ):
                    {
                        const OUString sXValue = rAttribs.getStringDefaulted( XML_x );
                        const OUString sYValue = rAttribs.getStringDefaulted( XML_y );

                        if( !sXValue.isEmpty() && nType != XML_ellipse )
                            mrShape.getTextBody()->getTextProperties().moTextOffLeft = GetCoordinate( sXValue.toInt32() - mrShape.getPosition().X );
                        if( !sYValue.isEmpty() )
                            mrShape.getTextBody()->getTextProperties().moTextOffUpper = GetCoordinate( sYValue.toInt32() - mrShape.getPosition().Y );
                    }
                    break;
                case A_TOKEN( ext ):
                    {
                        const OUString sXValue = rAttribs.getStringDefaulted( XML_cx );
                        const OUString sYValue = rAttribs.getStringDefaulted( XML_cy );

                        if( !sXValue.isEmpty() && nType == XML_rect )
                        {
                            mrShape.getTextBody()->getTextProperties().moTextOffRight = GetCoordinate(mrShape.getSize().Width - sXValue.toInt32());
                            if( mrShape.getTextBody()->getTextProperties().moTextOffLeft )
                               *mrShape.getTextBody()->getTextProperties().moTextOffRight -=  *mrShape.getTextBody()->getTextProperties().moTextOffLeft;
                        }
                        if( !sYValue.isEmpty() )
                        {
                            mrShape.getTextBody()->getTextProperties().moTextOffLower = GetCoordinate(mrShape.getSize().Height - sYValue.toInt32());
                            if( mrShape.getTextBody()->getTextProperties().moTextOffUpper )
                               *mrShape.getTextBody()->getTextProperties().moTextOffLower -=  *mrShape.getTextBody()->getTextProperties().moTextOffUpper;

                        }
                    }
                    break;
            }
        }
        return nullptr;
    }

    switch( aElementToken )
    {
    case A_TOKEN( off ):        // horz/vert translation
        mrShape.setPosition( awt::Point( rAttribs.getInteger( XML_x, 0 ), rAttribs.getInteger( XML_y, 0 ) ) );
        break;
    case A_TOKEN( ext ):        // horz/vert size
        mrShape.setSize( awt::Size( rAttribs.getInteger( XML_cx, 0 ), rAttribs.getInteger( XML_cy, 0 ) ) );
        break;
    case A_TOKEN( chOff ):  // horz/vert translation of children
        mrShape.setChildPosition( awt::Point( rAttribs.getInteger( XML_x, 0 ), rAttribs.getInteger( XML_y, 0 ) ) );
        break;
    case A_TOKEN( chExt ):  // horz/vert size of children
        {
            sal_Int32 nChExtCx = rAttribs.getInteger(XML_cx, 0);

            if(nChExtCx == 0)
                nChExtCx = mrShape.getSize().Width;

            sal_Int32 nChExtCy = rAttribs.getInteger(XML_cy, 0);

            if(nChExtCy == 0)
                nChExtCy = mrShape.getSize().Height;

            mrShape.setChildSize(awt::Size(nChExtCx, nChExtCy));
        }
        break;
    }

    return nullptr;
}

} // namespace oox::drawingml

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
