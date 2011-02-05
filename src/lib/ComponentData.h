/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COMPONENTDATA_H
#define __BOI_COMPONENTDATA_H


#include <QAtomicInt>
#include "CRef.h"
#include "Mutex.h"
#include "Connections.h"
#include "GraphicsItem.h"
#include "ViewLayerId.h"


namespace BOI {


class CSI;
class ISI;
class Task;
class Component;


class ComponentData
{
    public:
        ComponentData(Component* pComponent);

    public:
        /*
         * ComponentManager related members.
         */
        ComponentData* pNext;
        ComponentData* pPrev;
        CSI* pCSI;

        ISI* pISI;

        /*
         * This CRef is used by the system to assure that
         * the component instance stays around for as long
         * as the system needs it to. All classes except
         * ComponentManager should use this member as a
         * read-only value.
         */
        CRef cref;

        /*
         * CRef related members.
         */
        QAtomicInt refCount;
        Mutex      crefLock;
        int        instanceCount;
        bool       needsDestroy;

        /*
         * The system wide unique ID given to
         * the component when it is created.
         */
        int id;

        int flags;

        Component* pComponent;

        GraphicsItem graphicsItem;

        ViewLayerId layer;

        Mutex drawLock;

        Connections connections;

        /*
         * TaskProcessor related members.
         */
        Mutex tasksLock;
        Task* pTasksHead;
        Task* pTasksTail;
        QAtomicInt numActiveTasks;

        /*
         * The following members should only
         * be accessed or modified from the
         * main gui thread.
         */

        bool selected;
        bool visible;
        bool inLayout;
};


} // namespace BOI


#endif //__BOI_COMPONENTDATA_H

