/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_KINETICSCROLLVIEWTRANSFORMER_H
#define __BOI_KINETICSCROLLVIEWTRANSFORMER_H


#include <QTime>
#include <QLineF>
#include "ViewTransformer.h"


namespace BOI {


/*
 * Version 1
 *      arg[0] = The x component of the direction vector.
 *      arg[1] = The y component of the direction vector.
 *      arg[2] = The initial velocity in pixels/sec.
 *      arg[3] = The acceleration in pixels/sec^2.
 */

class KineticScrollViewTransformer
    : public ViewTransformer
{
    public:
        KineticScrollViewTransformer();

        virtual bool Initialize(int version, qreal* pArgs);

        virtual bool Run();

    private:
        QLineF m_directionVector;

        qreal m_initialVelocity;
        qreal m_acceleration;

        qreal m_finalTime;
        qreal m_distance;

        QTime m_time;
};


} // namespace BOI


#endif //__BOI_KINETICSCROLLVIEWTRANSFORMER_H

