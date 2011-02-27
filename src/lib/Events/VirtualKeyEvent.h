/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_VIRTUALKEYEVENT_H
#define __BOI_VIRTUALKEYEVENT_H


#include <QEvent>


namespace BOI {


class VirtualKeyEvent
    : public QEvent
{
    public:
        enum
        {
            Type_Press = 0,
            Type_Release
        };

    public:
        int type;
        int shiftKey;
        int defaultKey;

    private:
        friend class CustomEventsFactory;
        VirtualKeyEvent(int type);
};


inline VirtualKeyEvent::VirtualKeyEvent(int type)
    : QEvent((QEvent::Type)type)
{
}


} // namespace BOI


#endif //__BOI_VIRTUALKEYEVENT_H

