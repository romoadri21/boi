/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_GUIREQUESTEVENT_H
#define __BOI_GUIREQUESTEVENT_H


#include <QEvent>


namespace BOI {


class GuiRequestEvent
    : public QEvent
{
    private:
        friend class CustomEventsFactory;
        GuiRequestEvent(int type);
};


inline GuiRequestEvent::GuiRequestEvent(int type)
    : QEvent((QEvent::Type)type)
{
}


} // namespace BOI


#endif //__BOI_GUIREQUESTEVENT_H

