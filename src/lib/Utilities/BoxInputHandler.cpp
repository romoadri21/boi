/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "Utilities/BoxInputHandler.h"


namespace BOI {


BoxInputHandler::BoxInputHandler()
    : m_startPoint(),
      m_rect()
{
}


void BoxInputHandler::SetStartPoint(const QPointF& startPoint)
{
    m_startPoint = startPoint;
}


void BoxInputHandler::SetEndPoint(const QPointF& endPoint)
{
    qreal newWidth = endPoint.x() - m_startPoint.x();
    if (newWidth < 0)
    {
        newWidth *= -1;
    }

    qreal newHeight = endPoint.y() - m_startPoint.y();
    if (newHeight < 0)
    {
        newHeight *= -1;
    }

    m_rect.setRect(0, 0, newWidth, newHeight);

    if (endPoint.x() >= m_startPoint.x())
    {
        if (endPoint.y() < m_startPoint.y())
        {
            m_rect.moveTo(m_startPoint.x(), endPoint.y());
        }
        else
        {
            m_rect.moveTo(m_startPoint.x(), m_startPoint.y());
        }
    }
    else
    {
        if (endPoint.y() < m_startPoint.y())
        {
            m_rect.moveTo(endPoint.x(), endPoint.y());
        }
        else
        {
            m_rect.moveTo(endPoint.x(), m_startPoint.y());
        }
    }
}


const QRectF& BoxInputHandler::Rect()
{
    return m_rect;
}


} // namespace BOI

