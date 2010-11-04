/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QApplication>
#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif
#include "Instance.h"


int main(int argc, char *argv[])
{
// TODO: is this still necessary with the latest versions of X11?
#ifdef Q_WS_X11
    /*
     * Setup threading in the X11 environment.
     */
    XInitThreads();
#endif

    QApplication app(argc, argv);
    BOI::Instance boi;

    if (boi.Initialize(&app))
    {
        boi.Start();
    }
    else
    {
        return 0;
    }

    return app.exec();
} 

