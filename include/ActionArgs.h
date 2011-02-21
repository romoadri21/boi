/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_ACTIONARGS_H
#define __BOI_ACTIONARGS_H


#include <QMap>
#include <QString>
#include <QVariant>
#include "BOILib.h"


namespace BOI {


class BOI_LIB_EXPORT ActionArgs
{
    public:
        ActionArgs();

        void SetAutoDelete(bool autoDelete);
        bool AutoDelete() const;

        void Reset();

        void Set(const QString& argName,
                 const QVariant& value);

        void SetPtr(const QString& argName, void* value);

        int  NumArgs() const;
        bool Contains(const QString& argName) const;

        template<typename T>
        T Value(const QString& argName) const;

        template<typename T>
        T* ValuePtr(const QString& argName) const;

        void Remove(const QString& argName);

    private:
        bool m_autoDelete;
        QMap<QString, QVariant> m_args;
};


inline ActionArgs::ActionArgs()
    : m_autoDelete(true),
      m_args()
{
}


inline void ActionArgs::SetAutoDelete(bool autoDelete)
{
    m_autoDelete = autoDelete;
}


inline bool ActionArgs::AutoDelete() const
{
    return m_autoDelete;
}


inline void ActionArgs::Reset()
{
    m_args.clear();
}


inline void ActionArgs::Set(const QString& argName,
                            const QVariant& value)
{
    m_args.insert(argName, value);
}


inline void ActionArgs::SetPtr(const QString& argName, void* value)
{
    m_args.insert(argName, QVariant::fromValue(value));
}


inline int ActionArgs::NumArgs() const
{
    return m_args.size();
}


inline bool ActionArgs::Contains(const QString& argName) const
{
    return m_args.contains(argName);
}


template<typename T>
inline T ActionArgs::Value(const QString& argName) const
{
    return m_args.value(argName).value<T>();
}


template<typename T>
inline T* ActionArgs::ValuePtr(const QString& argName) const
{
    return (T*)m_args.value(argName).value<void*>();
}


inline void ActionArgs::Remove(const QString& argName)
{
    m_args.remove(argName);
}


} // namespace BOI


#endif //__BOI_ACTIONARGS_H

