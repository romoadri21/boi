/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include "MenuItem.h"
#include "StandardMenus.h"
#include "MenuRegistrar.h"
#include "FactoryRegistrar.h"
#include "BasicShapesPlugin.h"
#include "DescriptionRegistrar.h"


void BasicShapesPlugin::RegisterDataFactories(BOI::FactoryRegistrar* pRegistrar)
{
    Q_UNUSED(pRegistrar);
}


void BasicShapesPlugin::RegisterActionFactories(BOI::FactoryRegistrar* pRegistrar)
{
    pRegistrar->RegisterFactory(&m_actionsFactory);
}


void BasicShapesPlugin::RegisterComponentFactories(BOI::FactoryRegistrar* pRegistrar)
{
    pRegistrar->RegisterFactory(&m_componentsFactory);
}


void BasicShapesPlugin::RegisterMenus(BOI::MenuRegistrar* pRegistrar)
{
    BOI::MenuItem* pItem = pRegistrar->GetMenu(BOI_UUID_M(Root));
    pItem = pItem->FindSibling("Insert");

    BOI::MenuItem* pShapeItem = pItem->FindChild("Shape");

    if (pShapeItem == NULL)
    {
        pShapeItem = pItem->AddChild("Shape");
    }


    if (pShapeItem->FindChild("Rect") == NULL)
    {
        BOI::ActionArgs args;
        args.Set("UUID", "{5899283b-cfd6-4dfc-9544-4a4608d27fd9}");
        pShapeItem->AddChild("Rect", "{cbbbdce0-542f-40a4-a558-79c02e7ec88f}", args);
    }
}


void BasicShapesPlugin::RegisterDescriptions(BOI::DescriptionRegistrar* pRegistrar)
{
    Q_UNUSED(pRegistrar);
}


Q_EXPORT_PLUGIN2(BasicShapes, BasicShapesPlugin);

