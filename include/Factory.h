/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_FACTORY_H
#define __BOI_FACTORY_H


namespace BOI {


class Object;
class TypeRegistrar;


/*
 * Macros to assist with creating and referencing
 * the GetInstance and DeleteInstance functions.
 */

#define DEFINE_BOI_GET_INSTANCE_FUNC(ClassName, FunctionName) \
    static inline BOI::Object* FunctionName ## _Static(void* pFactory, int type) \
    { \
        return ((ClassName*)pFactory)->FunctionName(type); \
    } \
    BOI::Object* FunctionName(int type)

#define BOI_GET_INSTANCE_FUNC(ClassName, FunctionName) \
    ClassName::FunctionName##_Static


#define DEFINE_BOI_DELETE_INSTANCE_FUNC(ClassName, FunctionName) \
    static inline void FunctionName ## _Static(void* pFactory, BOI::Object* pInstance) \
    { \
        ((ClassName*)pFactory)->FunctionName(pInstance); \
    } \
    void FunctionName(BOI::Object* pInstance)

#define BOI_DELETE_INSTANCE_FUNC(ClassName, FunctionName) \
    ClassName::FunctionName##_Static


class Factory
{
    public:
        /*
         * The internal function pointers/prototypes for
         * the GetInstance and DeleteInstance functions that
         * a Factory supplies to the system so that the system
         * can create and delete instances of a specific type.
         */
        typedef Object* (*GetInstanceFunc)   (void* pFactory, int     type);
        typedef void    (*DeleteInstanceFunc)(void* pFactory, Object* pInstance);

    public:
        virtual void RegisterTypes(TypeRegistrar* pRegistrar) = 0;
};


} // namespace BOI


#endif //__BOI_FACTORY_H

