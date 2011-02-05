/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_GUIREQUEST_H
#define __BOI_GUIREQUEST_H


#include <QtGlobal>
#include "ViewLayerId.h"
#include "CRef.h"


namespace BOI {


class ComponentData;


class GuiRequest
{
    public:
        enum
        {
            RequestType_MoveToLayer = 0,
            RequestType_DestroyComponent,
            RequestType_SetBoundingRect,
            RequestType_CenterComponentOn,
            RequestType_SetTransformOrigin,
            RequestType_SetRotation,
            RequestType_SetPosition,
            RequestType_StackOnTop,
            RequestType_SetVisible,
            RequestType_SetOpacity,
            RequestType_SetParent,
            RequestType_UpdateRect,
            RequestType_Update,
            RequestType_Emit,
            RequestType_Rotate,
            RequestType_SetFlag,

            NumRequests
        };

    public:
        int  type;

        CRef cref;
        CRef cref2;

        union
        {
            ViewLayerId viewLayerId;

            ComponentData* pComponentData;

            qreal opacity;
            qreal rotation;

            bool boolean;

            struct
            {
                qreal x;
                qreal y;
            } point;

            struct
            {
                qreal x;
                qreal y;
                qreal width;
                qreal height;
            } rect;

            struct
            {
                int flag;
                bool enabled;
            } flagData;

            struct
            {
                int emitter;
                bool newOnly;
            } emitData;

            struct
            {
                bool relativeToScene;
                qreal x;
                qreal y;
            } centerComponentData;
        } data;

        GuiRequest* pNext;

    public:
        GuiRequest();
        GuiRequest(const GuiRequest& request);

        GuiRequest& operator=(const GuiRequest& request);
};


} // namespace BOI


#endif //__BOI_GUIREQUEST_H

