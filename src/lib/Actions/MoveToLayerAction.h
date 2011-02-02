/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MOVETOLAYERACTION_H
#define __BOI_MOVETOLAYERACTION_H


#include "Action.h"


/*
 * Arguments:
 * ----------
 *  Layer = The ViewLayerId of the layer to move
 *          the component to.
 */


namespace BOI {


class MoveToLayerAction
    : public Action
{
    public:
        MoveToLayerAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);
};


} // namespace BOI


#endif //__BOI_MOVETOLAYERACTION_H

