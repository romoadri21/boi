/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_INSTANCE_H
#define __BOI_INSTANCE_H


#include <QString>
#include "BOILib.h"


class QWidget;
class QCoreApplication;


namespace BOI {


class Base;


class BOI_LIB_EXPORT Instance
{
    public:
        Instance();
        ~Instance();

        bool Initialize(QCoreApplication* pApp,
                        QWidget* pParent = NULL,
                        int numTaskThreads = 0,
                        const QString& rootDir = QString(),
                        const QString& pluginsDir = QString());

        void Start();

    private:
        Base* m_pBase;
};


} // namespace BOI


#endif //__BOI_INSTANCE_H

