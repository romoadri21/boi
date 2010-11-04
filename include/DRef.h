/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_DREF_H
#define __BOI_DREF_H


#include "Data.h"
#include "BOILib.h"
#include "StandardDataTypes.h"


namespace BOI {


/*
 * Usage rules:
 * ------------
 * 
 *  1. No more than one thread should ever access the same
 *      DRef at the same time. If more than one thread
 *      needs to access the same DRef at the same time, a
 *      mutex (or some other concurrent locking mechanism) should
 *      be used. Multiple threads can access *separate* copies of
 *      the same DRef (which point to the same Data) at the same time.
 *  2. DRef should always be used and passed by value to
 *      maintain the integrity of the underlying reference counting.
 *  3. Pointers and references to this type should be avoided since
 *      that potentially might ruin the integrity of the underlying
 *      reference counting.
 *
 * Examples:
 * ---------
 *
 * A typical emitter scenario where the emitter holds on
 * to the DRef for its lifetime:
 *
 *      [inside of class definition]
 *      ...
 *      DRef m_widthRef;
 *      ...
 *
 *      [inside of class initialization]
 *      ...
 *      m_widthRef = SI()->NewData(BOI_STD_D(Int));
 *      ...
 *
 *      [inside of a method that emits the int value]
 *      ...
 *      while (doLoop)
 *      {
 *          int* pInt = m_widthRef.GetWriteInstance<int>();
 *          *pInt = newValue++;
 *          Emit(WidthEmitterEnum, m_widthRef);
 *          ...
 *      }
 *
 * Notice, in the while loop above, the instance is retrieved
 * every pass through the loop since it is possible that another
 * reference to the underlying Data object exists as a 
 * consequence/side effect of the Emit call (ie. a separate thread
 * might still be in the process of reading it and, changing it's
 * value at the same time could cause data corruption).
 * An instance returned by GetWriteInstance() should not
 * be used after the DRef has been passed to a function since that
 * function may have made a copy of the DRef. Getting a new 'writable'
 * instance from DRef::GetWriteInstance() will guarantee that the
 * returned instance is not simultaneously being read/written by another
 * thread or copy some where else.
 *
 * In the next example, the component releases its DRef inside of the
 * Emit(...) call which allows a single receiver to take full ownership
 * of the data and write to it without the need for a new/copy as might
 * be desired in an audio/video plugin chain. Note, however, if there
 * is more than one receiver connected to this emitter then the DRef
 * will not be released until all emits have been queued and the receivers
 * may not be able to take full ownership since more than one copy of the
 * DRef might be in existence.
 *
 *      [inside of a component method that emits an int value]
 *      ...
 *      while (doLoop)
 *      {
 *          DRef widthRef = SI()->NewData(BOI_STD_D(Int));
 *          int* pWidth = widthRef.GetWriteInstance<int>();
 *          *pWidth = newWidth;
 *          Emit(WidthEmitterEnum, widthRef, true);
 *          ...
 *          // Note, the widthRef is no longer valid because it was
 *          // released/Reset inside Emit(...).
 *      }
 *
 * There is no type checking done when retrieving an instance of the underlying
 * data that a DRef references. Thus, it is up to the programmer to respect
 * the 'type contract' of a particular connection and pass the correct Data type
 * to Emit(...) and Call(...). Violating this contract is considered programmer
 * error and should be avoided.
 *
 * If desired, basic type checking can be done using DRef::Type() which returns
 * the system internal type of the referenced Data.
 */
class BOI_LIB_EXPORT DRef
{
    public:
        DRef();
        DRef(const DRef& dref);
        ~DRef();

        /*
         * Returns true if this DRef references
         * a valid Data object and false otherwise.
         * GetReadInstance(), ReleaseInstance() and
         * DestroyInstance() should not be called
         * on an invalid DRef.
         */
        bool IsValid();

        /*
         * Returns the system internal type of the
         * referenced Data.
         */
        int Type();

        /* 
         * GetReadInstance() and GetWriteInstance() should only be called
         * on a valid DRef (one that references a valid Data object).
         * Use IsValid() to determine whether or not a DRef is valid. 
         * The instance returned by these functions is only valid while
         * the DRef references the specific Data object. Instances should
         * never be used after the DRef has been deleted, gone out of scope
         * or passed to a function.
         */
        template<typename T>
        const T* GetReadInstance();

        template<typename T>
        T* GetWriteInstance();

        /*
         * TryGetWriteInstance() returns the instance of the data that
         * this DRef references if the instance can be retreived immediately
         * (ie. no copy or extra processing needs to be performed).
         * Otherwise, it returns NULL.
         */
        template<typename T>
        T* TryGetWriteInstance();

        void Reset();

        DRef& operator=(const DRef& dref);

    private:
        /*
         * Only DataManager should be able to call this method so
         * that no other code can create a DRef that references
         * a Data object.
         */
        friend class DataManager;
        DRef(Data* pData);

        void* WriteInstance(bool doCopy);

    private:
        Data* m_pData;
};


inline DRef::DRef()
    : m_pData(NULL)
{
}


inline DRef::~DRef()
{
    Reset();
}


inline bool DRef::IsValid()
{
    return (m_pData != NULL);
}


inline int DRef::Type()
{
    return (m_pData != NULL) ? m_pData->Type() : BOI_STD_D(Invalid);
}


template<typename T>
inline const T* DRef::GetReadInstance()
{
    return (const T*) m_pData->Instance();
}


template<typename T>
inline T* DRef::GetWriteInstance()
{
    return (T*) WriteInstance(true);
}


template<typename T>
inline T* DRef::TryGetWriteInstance()
{
    return (T*) WriteInstance(false);
}


} // namespace BOI


#endif //__BOI_DREF_H

