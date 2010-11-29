/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_ZOOMACTION_H
#define __BOI_ZOOMACTION_H


#include <QPoint>
#include <QPointF>
#include "Action.h"


namespace BOI {


class ZoomAction
    : public Action
{
    public:
        ZoomAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

    private:
        int m_startY;
        int m_numTouchStreams;

        QPoint  m_viewPoint;
        QPointF m_layerPoint;

        float m_sensitivity;
};


} // namespace BOI


#endif //__BOI_ZOOMACTION_H

