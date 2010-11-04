/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_DATA_H
#define __BOI_DATA_H


#include <QDataStream>
#include <QAtomicInt>
#include "Object.h"


namespace BOI {


class DataManager;


class Data
    : public Object
{
    public:
        Data(int type);

        /* 
         * A virtual destructor is required so that the
         * destructor in the derived class gets called.
         */
        virtual ~Data() {}

        /*
         * Returns a pointer to the actual data
         * that this Data object contains.
         */
        virtual void* Instance() = 0;

        /*
         * Copies the data that this Data object
         * contains to the Data object passed in.
         */
        virtual void CopyInstance(Data* pData) = 0;

        /*
         * Exports/Imports this Data object to
         * and from the passed in QDataStream.
         */
        virtual void Import(QDataStream& in) = 0;
        virtual void Export(QDataStream& out) = 0;

    private:
        /*
         * Force the use of the Data(int type) constructor
         * since all Data objects should have an assigned
         * type.
         */
        Data();

    private:
        friend class DRef;
        QAtomicInt m_refCount;

        friend class DataManager;
        DataManager* m_pDataManager;
};


inline Data::Data(int type)
    : Object(type),
      m_refCount(0),
      m_pDataManager(NULL)
{
}


} // namespace BOI


#endif //__BOI_DATA_H

