/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_DESCRIPTIONMANAGER_H
#define __BOI_DESCRIPTIONMANAGER_H


#include <QHash>
#include <QString>
#include "DescriptionRegistrar.h"


namespace BOI {


class Description;


class DescriptionManager
    : public DescriptionRegistrar
{
    public:
        DescriptionManager();
        ~DescriptionManager();

        void InitStandardDescriptions();

        virtual Description* GetDescription(const QString& uuidMajor,
                                            const QString& uuidMinor = QString());

        virtual void RegisterDescription(Description* pDescription);

    private:
        QHash<QString, Description*> m_descriptions;
};


} // namespace BOI


#endif //__BOI_DESCRIPTIONMANAGER_H

