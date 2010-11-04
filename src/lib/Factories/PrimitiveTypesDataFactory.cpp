/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "StandardDataTypes.h"
#include "DataTypes/IntData.h"
#include "DataTypes/CharData.h"
#include "DataTypes/BoolData.h"
#include "DataTypes/ShortData.h"
#include "DataTypes/FloatData.h"
#include "DataTypes/DoubleData.h"
#include "TypeRegistrar.h"
#include "Factories/PrimitiveTypesDataFactory.h"


namespace BOI {


PrimitiveTypesDataFactory::PrimitiveTypesDataFactory()
    : Factory()
{
}


void PrimitiveTypesDataFactory::RegisterTypes(TypeRegistrar* pRegistrar)
{
    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory,    GetIntData),
                             BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteIntData),
                             StandardDataTypes::Uuid(BOI_STD_D(Int)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory,    GetCharData),
                             BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteCharData),
                             StandardDataTypes::Uuid(BOI_STD_D(Char)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory,    GetBoolData),
                             BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteBoolData),
                             StandardDataTypes::Uuid(BOI_STD_D(Bool)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory,    GetShortData),
                             BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteShortData),
                             StandardDataTypes::Uuid(BOI_STD_D(Short)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory,    GetFloatData),
                             BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteFloatData),
                             StandardDataTypes::Uuid(BOI_STD_D(Float)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(PrimitiveTypesDataFactory,    GetDoubleData),
                             BOI_DELETE_INSTANCE_FUNC(PrimitiveTypesDataFactory, DeleteDoubleData),
                             StandardDataTypes::Uuid(BOI_STD_D(Double)));
}


Object* PrimitiveTypesDataFactory::GetIntData(int type)
{
    Q_UNUSED(type);

    IntData* pData = new IntData();
    return pData; 
}


Object* PrimitiveTypesDataFactory::GetCharData(int type)
{
    Q_UNUSED(type);

    CharData* pData = new CharData();
    return pData; 
}


Object* PrimitiveTypesDataFactory::GetBoolData(int type)
{
    Q_UNUSED(type);

    BoolData* pData = new BoolData();
    return pData; 
}


Object* PrimitiveTypesDataFactory::GetShortData(int type)
{
    Q_UNUSED(type);

    ShortData* pData = new ShortData();
    return pData; 
}


Object* PrimitiveTypesDataFactory::GetFloatData(int type)
{
    Q_UNUSED(type);

    FloatData* pData = new FloatData();
    return pData; 
}


Object* PrimitiveTypesDataFactory::GetDoubleData(int type)
{
    Q_UNUSED(type);

    DoubleData* pData = new DoubleData();
    return pData; 
}


void PrimitiveTypesDataFactory::DeleteIntData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void PrimitiveTypesDataFactory::DeleteCharData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void PrimitiveTypesDataFactory::DeleteBoolData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void PrimitiveTypesDataFactory::DeleteShortData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void PrimitiveTypesDataFactory::DeleteFloatData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void PrimitiveTypesDataFactory::DeleteDoubleData(Object* pInstance)
{
    delete (Data*)pInstance;
}


} // namespace BOI

