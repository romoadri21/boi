/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_BOILIB_H
#define __BOI_BOILIB_H


#include <QtGlobal>


#ifdef BOI_LIB
#   define BOI_LIB_EXPORT Q_DECL_EXPORT
#else
#   define BOI_LIB_EXPORT Q_DECL_IMPORT
#endif // BOI_LIB


#endif //__BOI_BOILIB_H

