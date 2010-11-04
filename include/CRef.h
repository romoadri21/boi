/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_CREF_H
#define __BOI_CREF_H


#include <QtGlobal>
#include <QMetaType>
#include "BOILib.h"


namespace BOI {


class Component;
class ComponentData;


/*
 * Usage rules:
 *  1. No more than one thread should ever access the same
 *      CRef at the same time. If more than one thread
 *      needs to access the same CRef at the same time, a
 *      mutex (or some other concurrent locking mechanism) should
 *      be used. Multiple threads can access *separate* copies of
 *      the same CRef (which point to the same component) at
 *      the same time.
 *  2. CRef should always be used and passed by value to
 *      maintain the integrity of the underlying reference counting.
 *  3. Pointers and references to this type should not be used
 *      as they potentially may ruin the integrity of the underlying
 *      reference counting.
 */
class BOI_LIB_EXPORT CRef
{
    public:
        CRef();
        CRef(const CRef& cref);
        ~CRef();

        /*
         * Returns true if this CRef references
         * a valid component and false otherwise.
         * GetInstance(), ReleaseInstance() and
         * DestroyInstance() should not be called
         * on an invalid CRef. If a CRef has been
         * Reset() then IsValid() will return false.
         */
        bool IsValid();

        /*
         * Returns the system internal type of the
         * referenced Component. If there is no
         * referenced Component (ie. this CRef was
         * reset) the method returns BOI_STD_C(Invalid).
         */
        int Type();

        /*
         * Returns the system wide unique ID that is
         * assigned to the referenced component or -1
         * if this CRef does not reference a component.
         */
        int Id();

        /*
         * Every successfull (non NULL return value) call
         * to GetInstance() must be followed by a call
         * to ReleaseInstance() on the same CRef before
         * any other method is used such as the copy constructor,
         * operator=() or Reset().
         * If GetInstance() returns a non NULL Component* then
         * the component referenced is guaranteed to exist and
         * be initialized and will not be destroyed until
         * at least the matching ReleaseInstance() is called.
         */
        Component* GetInstance();
        void ReleaseInstance();
        void DestroyInstance();

        /*
         * A quick yet sometimes inaccurate way to determine
         * whether or not the Component that this CRef references
         * has already been destroyed. If this returns true then
         * it is guaranteed that a later call to GetInstance()
         * will return NULL. If this returns false then a later
         * call to GetInstance() may or may not return NULL.
         * One common situation where this might be used is for
         * quickly determining whether or not a CRef should be
         * passed on for further processing (ie. if at some point
         * further on in an algorithm GetInstance will be called
         * on a CRef then it is more efficient to skip this CRef
         * if it has already been destroyed).
         */
        bool IsDestroyed();

        /*
         * Used to simulate a delete for the cases where
         * an CRef component does not actually get deleted
         * but should still perform similar actions. One case
         * where this would occur is when dequeueing from
         * a value based array where the array remains
         * resident in memory for a while and multiple
         * CRefs get assigned to a specific index
         * (ie. CRef or[3]; or[1] = A;).
         */
        void Reset();

        CRef& operator=(const CRef& cref);
        bool operator==(const CRef& cref);

    private:
        /*
         * The passed in Component can not already be referenced by
         * a CRef. Only ComponentManager should be able to call this
         * method so that no other code can create a CRef that
         * references a Component.
         */
        friend class ComponentManager;
        CRef(ComponentData* pComponentData);

    private:
        ComponentData* m_pComponentData;
};


inline CRef::CRef()
    : m_pComponentData(NULL)
{
}


inline CRef::~CRef()
{
    Reset();
}


inline bool CRef::IsValid()
{
    return (m_pComponentData != NULL);
}


inline bool CRef::operator==(const CRef& cref)
{
    return (m_pComponentData == cref.m_pComponentData);
}


} // namespace BOI


/*
 * Make the CRef type known to QMetaType so
 * that it can be used in a QVariant.
 */
Q_DECLARE_METATYPE(BOI::CRef)


#endif //__BOI_CREF_H

