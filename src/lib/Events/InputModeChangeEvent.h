/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_INPUTMODECHANGEEVENT_H
#define __BOI_INPUTMODECHANGEEVENT_H


#include <QEvent>
#include "InputMode.h"


namespace BOI {


class InputModeChangeEvent
    : public QEvent
{
    public:
        InputModeChangeEvent();

    public:
        InputMode mode;

    private:
        friend class CustomEventsFactory;
        InputModeChangeEvent(int type);
};


/*
 * The default constructor is for situations where the
 * event type is not required and the extra overhead
 * of new/delete is unnecessary (see View.cpp).
 */
inline InputModeChangeEvent::InputModeChangeEvent()
    : QEvent(QEvent::None)
{
}


inline InputModeChangeEvent::InputModeChangeEvent(int type)
    : QEvent((QEvent::Type)type)
{
}


} // namespace BOI


#endif //__BOI_INPUTMODECHANGEEVENT_H

