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

#pragma once

#include <svx/sdr/view/Enums.hxx>
#include <vcl/event.hxx>
#include <svx/svdmrkv.hxx>

class SvxURLField;

// helper class SdrViewEvent
struct SVXCORE_DLLPUBLIC SdrViewEvent
{
    SdrHdl* mpHdl;
    SdrObject* mpObj;
    SdrObject* mpRootObj; // mark this when SdrBeginTextEdit is executed
    SdrPageView* mpPV;
    const SvxURLField* mpURLField;

    Point maLogicPos;
    SdrHitKind meHit;
    SdrEventKind meEvent;

    sal_uInt16 mnMouseClicks;
    MouseEventModifiers mnMouseMode;
    sal_uInt16 mnMouseCode;
    sal_uInt16 mnHlplIdx;
    sal_uInt16 mnGlueId;

    bool mbMouseDown : 1;
    bool mbMouseUp : 1;
    bool mbIsAction : 1; // Action is active
    bool mbIsTextEdit : 1; // TextEdit runs currently
    bool mbAddMark : 1;
    bool mbUnmark : 1;
    bool mbPrevNextMark : 1;
    bool mbMarkPrev : 1;

public:
    SdrViewEvent()
        : mpHdl(nullptr)
        , mpObj(nullptr)
        , mpRootObj(nullptr)
        , mpPV(nullptr)
        , mpURLField(nullptr)
        , meHit(SdrHitKind::NONE)
        , meEvent(SdrEventKind::NONE)
        , mnMouseClicks(0)
        , mnMouseMode(MouseEventModifiers::NONE)
        , mnMouseCode(0)
        , mnHlplIdx(0)
        , mnGlueId(0)
        , mbMouseDown(false)
        , mbMouseUp(false)
        , mbIsAction(false)
        , mbIsTextEdit(false)
        , mbAddMark(false)
        , mbUnmark(false)
        , mbPrevNextMark(false)
        , mbMarkPrev(false)
    {
    }
};

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
