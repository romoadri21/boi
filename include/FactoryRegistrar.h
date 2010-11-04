/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_FACTORYREGISTRAR_H
#define __BOI_FACTORYREGISTRAR_H


namespace BOI {


class Factory;


class FactoryRegistrar
{
    public:
        virtual void RegisterFactory(Factory* pFactory) = 0;
};


} // namespace BOI


#endif //__BOI_FACTORYREGISTRAR_H

