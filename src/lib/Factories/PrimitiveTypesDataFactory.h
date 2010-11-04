/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_PRIMITIVETYPESDATAFACTORY_H
#define __BOI_PRIMITIVETYPESDATAFACTORY_H


#include "Factory.h"


namespace BOI {


class PrimitiveTypesDataFactory
    : public Factory
{
    public:
        PrimitiveTypesDataFactory();

        virtual void RegisterTypes(TypeRegistrar* pRegistrar);

        DEFINE_BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory, GetIntData);
        DEFINE_BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory, GetCharData);
        DEFINE_BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory, GetBoolData);
        DEFINE_BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory, GetShortData);
        DEFINE_BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory, GetFloatData);
        DEFINE_BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory, GetDoubleData);

        DEFINE_BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteIntData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteCharData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteBoolData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteShortData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteFloatData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteDoubleData);
};


} // namespace BOI


#endif //__BOI_PRIMITIVETYPESDATAFACTORY_H

