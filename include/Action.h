/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_ACTION_H
#define __BOI_ACTION_H


#include "BOILib.h"
#include "Object.h"
#include "ActionCommand.h"


namespace BOI {


class ASI;
class KeyEvent;
class TouchEvent;
class ActionArgs;


class BOI_LIB_EXPORT Action
    : public Object
{
    public:
        Action(int type);

        /* 
         * A virtual destructor is required so that the
         * destructor in the derived class gets called.
         */
        virtual ~Action() {}

        virtual ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        virtual void Stop(ASI* pSI);

        virtual bool Suspend(ASI* pSI);

        virtual ActionCommand Resume(ASI* pSI);

        virtual bool AcceptTouchStream();
        virtual bool AcceptKeyStream();

        virtual ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);
        virtual ActionCommand HandleKeyEvent(ASI* pSI, KeyEvent* pEvent);

        /*
         * This method will only be called for the target action
         * if the action specified by the caller is equal to
         * Action::Type() for the target action.
         * Furthermore, any derived class implementing this method
         * is responsible for determining whether or not the Update
         * was intended to be called on itself or another version
         * of itself (if the implementation supports multiple
         * simultaneous instances via Suspend()). 
         */
        virtual ActionCommand Update(ASI* pSI, const ActionArgs* pArgs);

        virtual void HandleViewTransformed(ASI* pSI);

        virtual void Destroy();

    private:
        /*
         * Force the use of the Action(int type) constructor
         * since all Action objects should have an assigned
         * type.
         */
        Action();
};


inline Action::Action(int type)
    : Object(type)
{
}


} // namespace BOI


#endif //__BOI_ACTION_H

