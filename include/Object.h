/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_OBJECT_H
#define __BOI_OBJECT_H


namespace BOI {


/*
 * The base for all objects in the system.
 */

class Object
{
    public:
        Object(int type);

        /* 
         * A virtual destructor is required so that the
         * destructor in the derived class gets called.
         */
        virtual ~Object() {}

        /*
         * Returns the system internal type of the Object.
         */
        int Type();

    private:
        /*
         * Force the use of the Object(int type) constructor
         * since all objects derived from Object should have
         * an assigned type.
         */
        Object();

    private:
        const int m_type;
};


inline Object::Object(int type)
    : m_type(type)
{
}


inline int Object::Type()
{
    return m_type;
}


} // namespace BOI


#endif //__BOI_OBJECT_H

