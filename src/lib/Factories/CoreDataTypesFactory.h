/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_COREDATATYPESFACTORY_H
#define __BOI_COREDATATYPESFACTORY_H


#include "Factory.h"


namespace BOI {


class CoreDataTypesFactory
    : public Factory
{
    public:
        CoreDataTypesFactory();

        virtual void RegisterTypes(TypeRegistrar* pRegistrar);

        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetStringData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetSizeData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetColorData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetBrushData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetFontData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetPenData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetPointData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetMenuSetData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetRectData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetBoundingBoxData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetImageData);
        DEFINE_BOI_GET_INSTANCE_FUNC(CoreDataTypesFactory, GetIntListData);

        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteStringData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteSizeData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteColorData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteBrushData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteFontData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeletePenData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeletePointData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteMenuSetData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteRectData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteBoundingBoxData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteImageData);
        DEFINE_BOI_DELETE_INSTANCE_FUNC(CoreDataTypesFactory, DeleteIntListData);
};


} // namespace BOI


#endif //__BOI_COREDATATYPESFACTORY_H

