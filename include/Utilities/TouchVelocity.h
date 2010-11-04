/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TOUCHVELOCITY_H
#define __BOI_TOUCHVELOCITY_H


#include <QTime>
#include <QPointF>
#include "BOILib.h"


#define BOI_TOUCHVELOCITY_MAX_VECTORS 10
#define BOI_TOUCHVELOCITY_MAX_VECTOR_MAGNITUDE 4000


namespace BOI {


class BOI_LIB_EXPORT TouchVelocity
{
    public:
        TouchVelocity();

        void Reset();
        void AddTouch(qreal x, qreal y);

        /*
         * Returns the magnitude of the velocity
         * of the most recently added touch in
         * pixels/sec.
         */
        qreal InstantaneousMagnitude();

        /*
         * Returns the average magnitude
         * of the velocity in pixels/sec.
         */
        qreal Magnitude();

        /*
         * Returns the average direction of
         * the velocity starting at (0, 0).
         */
        QPointF Direction();

    private:
        typedef struct TouchData
        {
            qreal x;
            qreal y;
            int elapsedTime;
        } TouchData;

    private:
        void ComputeVelocity();

    private:
        TouchData m_touchData[BOI_TOUCHVELOCITY_MAX_VECTORS + 1];

        QTime m_time;

        int m_first;
        int m_index;
        int m_numTouches;

        bool m_dirty;

        qreal   m_magnitude;
        QPointF m_direction;
};


} // namespace BOI


#endif //__BOI_TOUCHVELOCITY_H

