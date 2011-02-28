/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "TypeRegistrar.h"
#include "StandardDataTypes.h"
#include "DataTypes/PenData.h"
#include "DataTypes/ListData.h"
#include "DataTypes/SizeData.h"
#include "DataTypes/FontData.h"
#include "DataTypes/RectData.h"
#include "DataTypes/PointData.h"
#include "DataTypes/ColorData.h"
#include "DataTypes/BrushData.h"
#include "DataTypes/ImageData.h"
#include "DataTypes/StringData.h"
#include "DataTypes/MenuSetData.h"
#include "DataTypes/BoundingBoxData.h"
#include "Factories/CoreDataTypesFactory.h"


namespace BOI {


CoreDataTypesFactory::CoreDataTypesFactory()
    : Factory()
{
}


void CoreDataTypesFactory::RegisterTypes(TypeRegistrar* pRegistrar)
{
    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetStringData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteStringData),
                             StandardDataTypes::Uuid(BOI_STD_D(String)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetSizeData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteSizeData),
                             StandardDataTypes::Uuid(BOI_STD_D(Size)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetColorData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteColorData),
                             StandardDataTypes::Uuid(BOI_STD_D(Color)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetBrushData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteBrushData),
                             StandardDataTypes::Uuid(BOI_STD_D(Brush)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetFontData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteFontData),
                             StandardDataTypes::Uuid(BOI_STD_D(Font)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetPenData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeletePenData),
                             StandardDataTypes::Uuid(BOI_STD_D(Pen)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetPointData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeletePointData),
                             StandardDataTypes::Uuid(BOI_STD_D(Point)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetMenuSetData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteMenuSetData),
                             StandardDataTypes::Uuid(BOI_STD_D(MenuSet)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetRectData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteRectData),
                             StandardDataTypes::Uuid(BOI_STD_D(Rect)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetBoundingBoxData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteBoundingBoxData),
                             StandardDataTypes::Uuid(BOI_STD_D(BoundingBox)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetImageData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteImageData),
                             StandardDataTypes::Uuid(BOI_STD_D(Image)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory,    GetIntListData),
                             BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteIntListData),
                             StandardDataTypes::Uuid(BOI_STD_D(IntList)));
}


Object* CoreDataTypesFactory::GetStringData(int type)
{
    Q_UNUSED(type);

    StringData* pData = new StringData();
    return pData; 
}


Object* CoreDataTypesFactory::GetSizeData(int type)
{
    Q_UNUSED(type);

    SizeData* pData = new SizeData();
    return pData; 
}


Object* CoreDataTypesFactory::GetColorData(int type)
{
    Q_UNUSED(type);

    ColorData* pData = new ColorData();
    return pData; 
}


Object* CoreDataTypesFactory::GetBrushData(int type)
{
    Q_UNUSED(type);

    BrushData* pData = new BrushData();
    return pData; 
}


Object* CoreDataTypesFactory::GetFontData(int type)
{
    Q_UNUSED(type);

    FontData* pData = new FontData();
    return pData;
}


Object* CoreDataTypesFactory::GetPenData(int type)
{
    Q_UNUSED(type);

    PenData* pData = new PenData();
    return pData;
}


Object* CoreDataTypesFactory::GetPointData(int type)
{
    Q_UNUSED(type);

    PointData* pData = new PointData();
    return pData;
}


Object* CoreDataTypesFactory::GetMenuSetData(int type)
{
    Q_UNUSED(type);

    MenuSetData* pData = new MenuSetData();
    return pData;
}


Object* CoreDataTypesFactory::GetRectData(int type)
{
    Q_UNUSED(type);

    RectData* pData = new RectData();
    return pData;
}


Object* CoreDataTypesFactory::GetBoundingBoxData(int type)
{
    Q_UNUSED(type);

    BoundingBoxData* pData = new BoundingBoxData();
    return pData;
}


Object* CoreDataTypesFactory::GetImageData(int type)
{
    Q_UNUSED(type);

    ImageData* pData = new ImageData();
    return pData;
}


Object* CoreDataTypesFactory::GetIntListData(int type)
{
    Q_UNUSED(type);

    ListData<int>* pData = new ListData<int>(BOI_STD_D(IntList));
    return pData;
}


void CoreDataTypesFactory::DeleteStringData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeleteSizeData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeleteColorData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeleteBrushData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeleteFontData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeletePenData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeletePointData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeleteMenuSetData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeleteRectData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeleteBoundingBoxData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeleteImageData(Object* pInstance)
{
    delete (Data*)pInstance;
}


void CoreDataTypesFactory::DeleteIntListData(Object* pInstance)
{
    delete (Data*)pInstance;
}


} // namespace BOI

