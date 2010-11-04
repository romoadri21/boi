/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TRANSFORMMANAGER_H
#define __BOI_TRANSFORMMANAGER_H


#include <QObject>
#include <QBasicTimer>
#include "ViewTransformerId.h"

/*
 * Update the view transform 20 times per second.
 */
#define BOI_VIEWTRANSFORM_UPDATE_INTERVAL 50


namespace BOI {


class View;
class ActionEngine;
class ViewTransformer;


class TransformManager
    : public QObject
{
    Q_OBJECT

    public:
        TransformManager(View* pView, ActionEngine* pActionEngine);
        ~TransformManager();

        int AppendTransformer(ViewTransformerId id,
                              int version,
                              qreal* pArgs);

        bool UpdateTransformer(int instanceId, qreal* pArgs);

        void StopTransformers();
        void StopTransformer(int instanceId);

    protected:
        void timerEvent(QTimerEvent *event);

    private:
        View* m_pView;

        QBasicTimer m_timer;

        ViewTransformer* m_pHead;
        ViewTransformer* m_pTail;

        int m_instanceId;

        ActionEngine* m_pActionEngine;
};


} // namespace BOI


#endif //__BOI_TRANSFORMMANAGER_H

