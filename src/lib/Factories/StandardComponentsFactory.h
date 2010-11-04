/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STANDARDCOMPONENTSFACTORY_H
#define __BOI_STANDARDCOMPONENTSFACTORY_H


#include "Factory.h"


namespace BOI {


class BasicComponentDrawData;


class StandardComponentsFactory
    : public Factory
{
    public:
        StandardComponentsFactory();
        ~StandardComponentsFactory();

        virtual void RegisterTypes(TypeRegistrar* pRegistrar);

        DEFINE_BOI_GET_INSTANCE_FUNC(StandardComponentsFactory, GetComponent);

    private:
        BasicComponentDrawData* m_pWindowSizeDrawData;
        BasicComponentDrawData* m_pVerticalLayoutDrawData;
        BasicComponentDrawData* m_pHorizontalLayoutDrawData;
};


} // namespace BOI


#endif //__BOI_STANDARDCOMPONENTSFACTORY_H

