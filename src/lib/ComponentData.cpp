/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "ComponentData.h"


namespace BOI {


ComponentData::ComponentData(Component* pComponentIn)
    : pNext(NULL),
      pPrev(NULL),
      pCSI(NULL),
      pISI(NULL),
      cref(),
      refCount(0),
      crefLock(),
      instanceCount(0),
      needsDestroy(false),
      pComponent(pComponentIn),
      graphicsItem(pComponentIn),
      layer(ViewLayerId_None),
      drawLock(),
      connections(),
      tasksLock(),
      pTasksHead(NULL),
      pTasksTail(NULL),
      numActiveTasks(1),
      selected(false),
      selectable(true),
      visible(false),
      inLayout(false)
{
}


} // namespace BOI

