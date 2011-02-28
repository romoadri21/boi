/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_LISTDATA_H
#define __BOI_LISTDATA_H


#include <QList>
#include "Data.h"


namespace BOI {


template <class T>
class ListData
    : public Data
{
    public:
        ListData(int type);

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        QList<T> m_list;
};


template <class T>
inline ListData<T>::ListData(int type)
    : Data(type)
{
}


template <class T>
inline void* ListData<T>::Instance()
{
    return &m_list;
}


template <class T>
inline void ListData<T>::CopyInstance(Data* pData)
{
    ((ListData<T>*)pData)->m_list = m_list;
}


template <class T>
inline void ListData<T>::Import(QDataStream& in)
{
    in >> m_list;
}


template <class T>
inline void ListData<T>::Export(QDataStream& out)
{
    out << m_list;
}


} // namespace BOI


#endif //__BOI_LISTDATA_H

