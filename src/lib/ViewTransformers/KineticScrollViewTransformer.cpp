/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "KineticScrollViewTransformer.h"


namespace BOI {


KineticScrollViewTransformer::KineticScrollViewTransformer()
    : ViewTransformer(),
      m_time()
{
}


bool KineticScrollViewTransformer::Initialize(int version, qreal* pArgs)
{
    Q_UNUSED(pArgs);

    if (version == 1)
    {
        m_directionVector.setLine(0, 0, pArgs[0], pArgs[1]);

        m_initialVelocity = pArgs[2];
        m_acceleration = pArgs[3];

        if (m_acceleration >= 0) return false;

        /*
         * Compute the final time given the initial velocity and
         * acceleration. This happens when the velocity is zero.
         * v(t) = 0 = v0 + a*t
         */
        m_finalTime = (-1.0 * m_initialVelocity) / m_acceleration;

        m_distance = 0;

        m_time.start();

        return true;
    }

    return false;
}


bool KineticScrollViewTransformer::Run()
{
    bool continueTransform = true;

    /*
     * Get the time in seconds that have elapsed
     * since the last call to start().
     */
    qreal elapsedTime = m_time.elapsed() / 1000.0;

    if (elapsedTime >= m_finalTime)
    {
        elapsedTime = m_finalTime;
        continueTransform = false;
    }

    /*
     * Calculate the total distance traveled given the elapsed time.
     * pos(t) = (v0 * t) + (1/2 * a * t^2)
     */
    qreal distance = (m_initialVelocity * elapsedTime) +
                     (0.5 * m_acceleration * (elapsedTime * elapsedTime));

    qreal delta = distance - m_distance;
    m_distance = distance;

    m_directionVector.setLength(delta);

    Translate(m_directionVector.x2(), m_directionVector.y2());

    return continueTransform;
}


} // namespace BOI

