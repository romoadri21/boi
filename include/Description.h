/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_DESCRIPTION_H
#define __BOI_DESCRIPTION_H


#include <QString>


namespace BOI {


class Description
{
    public:
        Description();

        void SetBody(const QString& body);
        void SetTitle(const QString& title);
        void SetUuidMajor(const QString& uuid);
        void SetUuidMinor(const QString& uuid);

        const QString& Body() const;
        const QString& Title() const;
        const QString& UuidMajor() const;
        const QString& UuidMinor() const;

    private:
        QString m_body;
        QString m_title;
        QString m_uuidMajor;
        QString m_uuidMinor;
};


inline Description::Description()
    : m_body(),
      m_title(),
      m_uuidMajor(),
      m_uuidMinor()
{
}


inline void Description::SetBody(const QString& body)
{
    m_body = body;
}


inline void Description::SetTitle(const QString& title)
{
    m_title = title;
}


inline void Description::SetUuidMajor(const QString& uuid)
{
    m_uuidMajor = uuid;
}


inline void Description::SetUuidMinor(const QString& uuid)
{
    m_uuidMinor = uuid;
}


inline const QString& Description::Body() const
{
    return m_body;
}


inline const QString& Description::Title() const
{
    return m_title;
}


inline const QString& Description::UuidMajor() const
{
    return m_uuidMajor;
}


inline const QString& Description::UuidMinor() const
{
    return m_uuidMinor;
}


} // namespace BOI


#endif //__BOI_DESCRIPTION_H

