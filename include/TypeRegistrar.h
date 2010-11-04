/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_TYPEREGISTRAR_H
#define __BOI_TYPEREGISTRAR_H


#include <QString>
#include "Factory.h"


namespace BOI {


class TypeRegistrar
{
    public:
        /*
         * Registers a particular type and provides function
         * pointers to create and delete instances of that type.
         * Note, the uuid parameter must be a lower case string and
         * in the form:
         *
         *      "{9757ee02-90b5-41c0-8ecf-5214a1d40bdb}"
         */
        virtual int RegisterType(void*                       pFactory,
                                 Factory::GetInstanceFunc    getInstanceFunc,
                                 Factory::DeleteInstanceFunc deleteInstanceFunc,
                                 const QString&              uuid) = 0;

        /*
         * Returns true if the type represented by the uuid
         * is registered and false otherwise.
         */
        virtual bool TypeRegistered(const QString& uuid) = 0;

        /*
         * Converts the uuid to the sytem internal ID
         * that represents this type.
         */
        virtual int ConvertUuid(const QString& uuid) = 0;
};


} // namespace BOI


#endif //__BOI_TYPEREGISTRAR_H

