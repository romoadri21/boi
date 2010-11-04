/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TUPLE_H
#define __BOI_TUPLE_H


namespace BOI {


template <class T1, class T2>
struct Tuple2
{
    Tuple2()
        : item1(T1()),
          item2(T2())
    {
    }

    Tuple2(const T1 &t1, const T2 &t2)
        : item1(t1),
          item2(t2)
    {
    }

    Tuple2<T1, T2>& operator=(const Tuple2<T1, T2>& other)
    {
        item1 = other.item1;
        item2 = other.item2;
        return *this;
    }

    T1 item1;
    T2 item2;
};


} // namespace BOI


#endif //__BOI_TUPLE_H

