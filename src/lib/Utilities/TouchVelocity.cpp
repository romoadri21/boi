/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QLineF>
#include "math.h"
#include "Utilities/TouchVelocity.h"


#define NEXT_INDEX(index) ((index + 1) % (BOI_TOUCHVELOCITY_MAX_VECTORS + 1))
#define PREV_INDEX(index) ((index == 0) ? BOI_TOUCHVELOCITY_MAX_VECTORS : (index - 1))


namespace BOI {


TouchVelocity::TouchVelocity()
{
    Reset();
}


void TouchVelocity::Reset()
{
    m_first = -1;
    m_index = 0;
    m_dirty = false;
    m_numTouches = 0;

    m_magnitude = 0.0;
    m_direction = QPointF(0, 0);
}


void TouchVelocity::AddTouch(qreal x, qreal y)
{
    int nextIndex = NEXT_INDEX(m_index);

    m_touchData[m_index].x = x;
    m_touchData[m_index].y = y;

    if (m_first == -1)
    {
        m_first = 0;
        m_numTouches++;

        m_time.start();
    }
    else
    {
        if (m_index == m_first)
        {
            m_first = nextIndex;
        }
        else
        {
            m_numTouches++;
        }

        /*
         * Set dirty to true only if there
         * is more then one touch since one
         * touch is not enough to determine
         * speed and direction.
         */
        m_dirty = true;

        m_touchData[m_index].elapsedTime = m_time.restart();
    }

    m_index = nextIndex;
}


void TouchVelocity::ComputeVelocity()
{
    QPointF vector(0, 0);

    qreal xPrev = m_touchData[m_first].x;
    qreal yPrev = m_touchData[m_first].y;

    int index = NEXT_INDEX(m_first);
    int numVectors = m_numTouches - 1;

    for (int i=0; i < numVectors; i++)
    {
        QLineF newVector(0, 0,
                         m_touchData[index].x - xPrev,
                         m_touchData[index].y - yPrev);

        if (m_touchData[index].elapsedTime == 0)
        {
            /*
             * Avoid divide by zero when
             * computing magnitude.
             */
            m_touchData[index].elapsedTime = 1;
        }

        qreal elapsedTime = m_touchData[index].elapsedTime / 1000.0;

        newVector.setLength(newVector.length() / elapsedTime);

        if (newVector.length() > BOI_TOUCHVELOCITY_MAX_VECTOR_MAGNITUDE)
        {
            /*
             * Level out the random spikes in magnitude.
             */
            newVector.setLength(BOI_TOUCHVELOCITY_MAX_VECTOR_MAGNITUDE);
        }

        vector += newVector.p2();

        xPrev = m_touchData[index].x;
        yPrev = m_touchData[index].y;

        index = NEXT_INDEX(index);
    }


    /*
     * Take the average of
     * all the vectors.
     */
    vector /= numVectors;

    m_direction = vector;
    m_magnitude = sqrt((vector.x() * vector.x()) + (vector.y() * vector.y()));

    m_dirty = false;
}


qreal TouchVelocity::InstantaneousMagnitude()
{
    if (m_numTouches < 2) return 0.0;

    int index = PREV_INDEX(m_index);

    qreal x2 = m_touchData[index].x;
    qreal y2 = m_touchData[index].y;

    if (m_touchData[index].elapsedTime == 0)
    {
        /*
         * Avoid divide by zero when
         * computing magnitude.
         */
        m_touchData[index].elapsedTime = 1;
    }

    qreal elapsedTime = m_touchData[index].elapsedTime / 1000.0;

    index = PREV_INDEX(index);

    qreal xDelta = x2 - m_touchData[index].x;
    qreal yDelta = y2 - m_touchData[index].y;

    qreal magnitude = sqrt((xDelta * xDelta) + (yDelta * yDelta));
    magnitude = magnitude / elapsedTime;

    if (magnitude > BOI_TOUCHVELOCITY_MAX_VECTOR_MAGNITUDE)
    {
        /*
         * Level out the random spikes in magnitude.
         */
        magnitude = BOI_TOUCHVELOCITY_MAX_VECTOR_MAGNITUDE;
    }

    return magnitude;
}


qreal TouchVelocity::Magnitude()
{
    if (m_dirty)
    {
        ComputeVelocity();
    }

    return m_magnitude;
}


QPointF TouchVelocity::Direction()
{
    if (m_dirty)
    {
        ComputeVelocity();
    }

    return m_direction;
}


} // namespace BOI

