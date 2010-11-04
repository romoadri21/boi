/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QLineF>
#include "ScrollViewTransformer.h"


namespace BOI {


ScrollViewTransformer::ScrollViewTransformer()
    : ViewTransformer(),
      m_xDelta(1),
      m_yDelta(1)
{
}


bool ScrollViewTransformer::Initialize(int version, qreal* pArgs)
{
    Q_UNUSED(pArgs);

    if (version == 1)
    {
        // TODO: make sure that both arg0
        // and arg1 are not zero.

        QLineF line(0, 0, pArgs[0], pArgs[1]);
        qreal length = (UpdateInterval() / 1000) * pArgs[2];
        line.setLength(length);

        m_xDelta = line.x2();
        m_yDelta = line.y2();

        return true;
    }

    return false;
}


bool ScrollViewTransformer::Run()
{
    Translate(m_xDelta, m_yDelta);
    return true;
}


void ScrollViewTransformer::Reset()
{
    m_xDelta = 1;
    m_yDelta = 1;
}


} // namespace BOI

