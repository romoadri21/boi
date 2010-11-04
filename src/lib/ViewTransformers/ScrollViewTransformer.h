/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_SCROLLVIEWTRANSFORMER_H
#define __BOI_SCROLLVIEWTRANSFORMER_H


#include "ViewTransformer.h"


namespace BOI {


/*
 * Version 1
 *      arg[0] = The x component of the direction vector.
 *      arg[1] = The y component of the direction vector.
 *      arg[2] = The distance in pixels that the scroll
 *               should advance per second. This value is
 *               local to the transform and will not take
 *               into account the current view scale or any
 *               other active ViewTransformers.
 */

class ScrollViewTransformer
    : public ViewTransformer
{
    public:
        ScrollViewTransformer();

        virtual bool Initialize(int version, qreal* pArgs);

        virtual bool Run();
        virtual void Reset();

    private:
        qreal m_xDelta;
        qreal m_yDelta;
};


} // namespace BOI


#endif //__BOI_SCROLLVIEWTRANSFORMER_H

