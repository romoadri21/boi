/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QGraphicsView>
#include "Scene.h"


namespace BOI {


Scene::Scene()
    : m_mainLayer(),
      m_systemLayer(),
      m_overlayLayer(),
      m_underlayLayer()
{
    m_mainLayer.setSceneRect(-30000000, -30000000, 60000000, 60000000);
    m_mainLayer.setItemIndexMethod(QGraphicsScene::NoIndex);

    m_systemLayer.setSceneRect(-30000000, -30000000, 60000000, 60000000);
    m_systemLayer.setItemIndexMethod(QGraphicsScene::NoIndex);

    m_overlayLayer.setSceneRect(-30000000, -30000000, 60000000, 60000000);
    m_overlayLayer.setItemIndexMethod(QGraphicsScene::NoIndex);

    m_underlayLayer.setSceneRect(-30000000, -30000000, 60000000, 60000000);
    m_underlayLayer.setItemIndexMethod(QGraphicsScene::NoIndex);
}


Scene::~Scene()
{
    QList<QGraphicsView*> views;

    views.append(m_mainLayer.views());
    views.append(m_systemLayer.views());
    views.append(m_overlayLayer.views());
    views.append(m_underlayLayer.views());

    for (int i=0; i < views.size(); i++)
    {
        views.at(i)->setScene(NULL);
    }
}


QGraphicsScene* Scene::Layer(SceneLayerId layer)
{
    if (layer == SceneLayerId_Main)
    {
        return &m_mainLayer;
    }
    else if (layer == SceneLayerId_System)
    {
        return &m_systemLayer;
    }
    else if (layer == SceneLayerId_Overlay)
    {
        return &m_overlayLayer;
    }
    else if (layer == SceneLayerId_Underlay)
    {
        return &m_underlayLayer;
    }

    return &m_mainLayer;
}


} // namespace BOI

