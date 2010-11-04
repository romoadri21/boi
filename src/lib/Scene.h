/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_SCENE_H
#define __BOI_SCENE_H


#include <QGraphicsScene>
#include "SceneLayerId.h"


namespace BOI {


class Scene
{
    public:
        Scene();
        ~Scene();

        QGraphicsScene* Layer(SceneLayerId layer);

    private:
        QGraphicsScene m_mainLayer;
        QGraphicsScene m_systemLayer;
        QGraphicsScene m_overlayLayer;
        QGraphicsScene m_underlayLayer;
};


} // namespace BOI


#endif //__BOI_SCENE_H

