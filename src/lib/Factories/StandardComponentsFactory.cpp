/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QtGlobal>
#include <QSizeF>
#include "TypeRegistrar.h"
#include "StandardComponents.h"
#include "Components/MenuComponent.h"
#include "Components/TextComponent.h"
#include "Components/BrowserComponent.h"
#include "Components/XGestureComponent.h"
#include "Components/TextInputComponent.h"
#include "Components/WindowSizeComponent.h"
#include "Components/JavascriptComponent.h"
#include "Components/BasicComponentDrawData.h"
#include "Components/VerticalLayoutComponent.h"
#include "Components/HorizontalLayoutComponent.h"
#include "Factories/StandardComponentsFactory.h"


namespace BOI {


StandardComponentsFactory::StandardComponentsFactory()
    : Factory()
{
    // TODO: put the draw data initialization in the component creation
    // part so it's only created when necessary. Use a mutex to stop it
    // from being created at the same time by multiple threads. Or use an
    // atomic pointer?
    m_pWindowSizeDrawData = new BasicComponentDrawData();
    m_pWindowSizeDrawData->SetSvg(":/BOI/WindowSize.svg", QSizeF(105, 105));

    m_pJavascriptDrawData = new BasicComponentDrawData();
    m_pJavascriptDrawData->SetSvg(":/BOI/JavascriptEngine.svg", QSizeF(105, 105));

    m_pVerticalLayoutDrawData = new BasicComponentDrawData();
    m_pVerticalLayoutDrawData->SetSvg(":/BOI/VerticalLayout.svg", QSizeF(105, 105));

    m_pHorizontalLayoutDrawData = new BasicComponentDrawData();
    m_pHorizontalLayoutDrawData->SetSvg(":/BOI/HorizontalLayout.svg", QSizeF(105, 105));
}


StandardComponentsFactory::~StandardComponentsFactory()
{
    delete m_pWindowSizeDrawData;
    delete m_pJavascriptDrawData;
    delete m_pVerticalLayoutDrawData;
    delete m_pHorizontalLayoutDrawData;
}


void StandardComponentsFactory::RegisterTypes(TypeRegistrar* pRegistrar)
{
    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(StandardComponentsFactory, GetComponent),
                             NULL,
                             StandardComponents::Uuid(BOI_STD_C(Menu)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(StandardComponentsFactory, GetComponent),
                             NULL,
                             StandardComponents::Uuid(BOI_STD_C(XGesture)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(StandardComponentsFactory, GetComponent),
                             NULL,
                             StandardComponents::Uuid(BOI_STD_C(WindowSize)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(StandardComponentsFactory, GetComponent),
                             NULL,
                             StandardComponents::Uuid(BOI_STD_C(VerticalLayout)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(StandardComponentsFactory, GetComponent),
                             NULL,
                             StandardComponents::Uuid(BOI_STD_C(HorizontalLayout)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(StandardComponentsFactory, GetComponent),
                             NULL,
                             StandardComponents::Uuid(BOI_STD_C(Text)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(StandardComponentsFactory, GetComponent),
                             NULL,
                             StandardComponents::Uuid(BOI_STD_C(TextInput)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(StandardComponentsFactory, GetComponent),
                             NULL,
                             StandardComponents::Uuid(BOI_STD_C(Browser)));

    pRegistrar->RegisterType(this,
                             BOI_GET_INSTANCE_FUNC(StandardComponentsFactory, GetComponent),
                             NULL,
                             StandardComponents::Uuid(BOI_STD_C(Javascript)));
}


Object* StandardComponentsFactory::GetComponent(int type)
{
    Component* pComponent = NULL;

    switch (type)
    {
        case BOI_STD_C(Menu):
            pComponent = new MenuComponent();
            break;

        case BOI_STD_C(XGesture):
            pComponent = new XGestureComponent();
            break;

        case BOI_STD_C(WindowSize):
            pComponent = new WindowSizeComponent(m_pWindowSizeDrawData);
            break;

        case BOI_STD_C(VerticalLayout):
            pComponent = new VerticalLayoutComponent(m_pVerticalLayoutDrawData);
            break;

        case BOI_STD_C(HorizontalLayout):
            pComponent = new HorizontalLayoutComponent(m_pHorizontalLayoutDrawData);
            break;

        case BOI_STD_C(Text):
            pComponent = new TextComponent();
            break;

        case BOI_STD_C(TextInput):
            pComponent = new TextInputComponent();
            break;

        case BOI_STD_C(Browser):
            pComponent = new BrowserComponent();
            break;

        case BOI_STD_C(Javascript):
            pComponent = new JavascriptComponent(m_pJavascriptDrawData);
            break;

        default:
            break;
    }

    return pComponent; 
}


} // namespace BOI

