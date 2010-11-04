/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_SETACTIONEVENT_H
#define __BOI_SETACTIONEVENT_H


#include <QEvent>


namespace BOI {


class ActionArgs;


class SetActionEvent
    : public QEvent
{
    public:
        int action;
        const ActionArgs* pArgs;

    private:
        friend class CustomEventsFactory;
        SetActionEvent(int type);
};


inline SetActionEvent::SetActionEvent(int type)
    : QEvent((QEvent::Type)type)
{
}


} // namespace BOI


#endif //__BOI_SETACTIONEVENT_H

