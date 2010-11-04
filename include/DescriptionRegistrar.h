/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_DESCRIPTIONREGISTRAR_H
#define __BOI_DESCRIPTIONREGISTRAR_H


#include <QString>


namespace BOI {


class Description;


class DescriptionRegistrar
{
    public:
        virtual Description* GetDescription(const QString& uuidMajor,
                                            const QString& uuidMinor = QString()) = 0;

        virtual void RegisterDescription(Description* pDescription) = 0;
};


} // namespace BOI


#endif //__BOI_DESCRIPTIONREGISTRAR_H

