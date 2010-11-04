/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_INTERFACE_H
#define __BOI_INTERFACE_H


#include <QtGlobal>
#include <QStringList>
#include "ProfileType.h"
#include "DRef.h"


namespace BOI {


// The internal function pointer/prototype for all receivers.
typedef void (*ReceiverFunc)(void*, DRef&, int);


#define BOI_DEFINE_RECEIVER_FUNC(ClassName, FunctionName) \
    static inline void FunctionName ## _Static(void* pComponent, BOI::DRef& dref, int source) \
    { \
        ((ClassName*)pComponent)->FunctionName(dref, source); \
    } \
    void FunctionName(BOI::DRef& dref, int source)

#define BOI_RECEIVER_FUNC(ClassName, FunctionName) \
    ClassName::FunctionName##_Static


#define BOI_BEGIN_RECEIVERS(ClassName) \
    const BOI::Interface::ReceiverInfo ClassName::ComponentInterface::s_receivers[] = { \
        {"{84e256a6-9291-451b-a0ff-f38602d2f839}", BOI_RECEIVER_FUNC(Component, SetPosition)}, \
        {"{af919167-3fd8-4fad-8990-cce279a8366c}", BOI_RECEIVER_FUNC(Component, SetRotation)}, \
        {"{0107f9b1-af4c-4b48-b772-c21c567cb53a}", BOI_RECEIVER_FUNC(Component, SetOpacity)}, \
        {"{f4e6a67a-8cd5-441c-a7c0-367c8360a146}", BOI_RECEIVER_FUNC(Component, SetVisible)},
#define BOI_DECLARE_RECEIVER(uuid, func) {uuid, func},
#define BOI_END_RECEIVERS(ClassName) \
    {"", NULL}}; \
    const int ClassName::ComponentInterface::s_numReceivers = \
        (sizeof(ClassName::ComponentInterface::s_receivers) / \
         sizeof(ClassName::ComponentInterface::s_receivers[0])) - 1;


#define BOI_BEGIN_EMITTERS(ClassName) \
    const BOI::Interface::EmitterInfo ClassName::ComponentInterface::s_emitters[] = { \
        {"{6fbe5587-9110-4b87-9049-2d5058d09304}"},
#define BOI_DECLARE_EMITTER(uuid) {uuid},
#define BOI_END_EMITTERS(ClassName) \
    {""}}; \
    const int ClassName::ComponentInterface::s_numEmitters = \
        (sizeof(ClassName::ComponentInterface::s_emitters) / \
         sizeof(ClassName::ComponentInterface::s_emitters[0])) - 1;


// The internal function pointer/prototype for all FuncSet functions.
typedef DRef (*FuncSetFunc)(void*, int, DRef&);


#define BOI_DEFINE_FUNCSET_FUNC(ClassName, FunctionName) \
    static inline BOI::DRef FunctionName ## _Static(void* pComponent, int id, BOI::DRef& dref) \
    { \
        return ((ClassName*)pComponent)->FunctionName(id, dref); \
    } \
    BOI::DRef FunctionName(int id, BOI::DRef& dref)

#define BOI_FUNCSET_FUNC(ClassName, FunctionName) \
    ClassName::FunctionName##_Static


#define BOI_MAX_FUNCSET_FUNCS 9

#define BOI_BEGIN_FUNCSETS(ClassName) \
    const BOI::Interface::FuncSetInfo ClassName::ComponentInterface::s_funcSets[] = {

#define BOI_DECLARE_FUNCSET_1(uuid, func1) \
    {uuid, 1, {func1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}},

#define BOI_DECLARE_FUNCSET_2(uuid, func1, func2) \
    {uuid, 2, {func1, func2, NULL, NULL, NULL, NULL, NULL, NULL, NULL}},

#define BOI_DECLARE_FUNCSET_3(uuid, func1, func2, func3) \
    {uuid, 3, {func1, func2, func3, NULL, NULL, NULL, NULL, NULL, NULL}},

#define BOI_DECLARE_FUNCSET_4(uuid, func1, func2, func3, func4) \
    {uuid, 4, {func1, func2, func3, func4, NULL, NULL, NULL, NULL, NULL}},

#define BOI_DECLARE_FUNCSET_5(uuid, func1, func2, func3, func4, func5) \
    {uuid, 5, {func1, func2, func3, func4, func5, NULL, NULL, NULL, NULL}},

#define BOI_DECLARE_FUNCSET_6(uuid, func1, func2, func3, func4, func5, func6) \
    {uuid, 6, {func1, func2, func3, func4, func5, func6, NULL, NULL, NULL}},

#define BOI_DECLARE_FUNCSET_7(uuid, func1, func2, func3, func4, func5, func6, func7) \
    {uuid, 7, {func1, func2, func3, func4, func5, func6, func7, NULL, NULL}},

#define BOI_DECLARE_FUNCSET_8(uuid, func1, func2, func3, func4, func5, func6, func7, func8) \
    {uuid, 8, {func1, func2, func3, func4, func5, func6, func7, func8, NULL}},

#define BOI_DECLARE_FUNCSET_9(uuid, func1, func2, func3, func4, func5, func6, func7, func8, func9) \
    {uuid, 9, {func1, func2, func3, func4, func5, func6, func7, func8, func9}},

#define BOI_END_FUNCSETS(ClassName) \
    {"", 0, {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}}}; \
    const int ClassName::ComponentInterface::s_numFuncSets = \
        (sizeof(ClassName::ComponentInterface::s_funcSets) / \
         sizeof(ClassName::ComponentInterface::s_funcSets[0])) - 1;


#define BOI_BEGIN_CALLERS(ClassName) \
    const BOI::Interface::CallerInfo ClassName::ComponentInterface::s_callers[] = {
#define BOI_DECLARE_CALLER(uuid) {uuid},
#define BOI_END_CALLERS(ClassName) \
    {""}}; \
    const int ClassName::ComponentInterface::s_numCallers = \
        (sizeof(ClassName::ComponentInterface::s_callers) / \
         sizeof(ClassName::ComponentInterface::s_callers[0])) - 1;


/*
 * This should be declared first in the Component definition
 * followed by receiver func definitions and FuncSetFunc definitions.
 */
#define BOI_INTERFACE_DEFINITION() \
    class ComponentInterface : public BOI::Interface \
    { \
        public: \
            virtual int NumCallers()   { return s_numCallers; } \
            virtual int NumFuncSets()  { return s_numFuncSets; } \
            virtual int NumEmitters()  { return s_numEmitters; } \
            virtual int NumReceivers() { return s_numReceivers; } \
        protected: \
            virtual const CallerInfo*   Callers()   { return s_callers; } \
            virtual const FuncSetInfo*  FuncSets()  { return s_funcSets; } \
            virtual const EmitterInfo*  Emitters()  { return s_emitters; } \
            virtual const ReceiverInfo* Receivers() { return s_receivers; } \
        private: \
            static const int s_numCallers; \
            static const int s_numFuncSets; \
            static const int s_numEmitters; \
            static const int s_numReceivers; \
            static const CallerInfo   s_callers[]; \
            static const FuncSetInfo  s_funcSets[]; \
            static const EmitterInfo  s_emitters[]; \
            static const ReceiverInfo s_receivers[]; \
    }; \
    ComponentInterface m_componentInterface; \
    protected: virtual BOI::Interface* IFace() { return &m_componentInterface; } private:


class Interface
{
    public:
        virtual int NumCallers()   = 0;
        virtual int NumFuncSets()  = 0;
        virtual int NumEmitters()  = 0;
        virtual int NumReceivers() = 0;

    protected:
        typedef struct
        {
            const char uuid[39];
            ReceiverFunc func;
        } ReceiverInfo;

        typedef struct
        {
            const char uuid[39];
        } EmitterInfo;

        typedef struct
        {
            const char uuid[39];

            int numFuncs;
            FuncSetFunc funcs[BOI_MAX_FUNCSET_FUNCS];
        } FuncSetInfo;

        typedef struct
        {
            const char uuid[39];
        } CallerInfo;

    protected:
        friend class Component;

        const QStringList GetProfile(ProfileType type=ProfileType_Full);

        int GetCaller(const QString& uuid);
        int GetFuncSet(const QString& uuid);
        int GetEmitter(const QString& uuid);
        int GetReceiver(const QString& uuid);

        ReceiverFunc GetReceiverFunc(int receiver);

        int NumFuncSetFuncs(int funcSet);
        const FuncSetFunc* GetFuncSetFuncs(int funcSet);

        /*
         * The following methods are for internal use only
         * and should only be called from within Interface.
         */
        virtual const CallerInfo*   Callers()   = 0;
        virtual const FuncSetInfo*  FuncSets()  = 0;
        virtual const EmitterInfo*  Emitters()  = 0;
        virtual const ReceiverInfo* Receivers() = 0;

    private:
        void InsertString(QStringList& list, QString& string);
};


} // namespace BOI


#endif //__BOI_INTERFACE_H

