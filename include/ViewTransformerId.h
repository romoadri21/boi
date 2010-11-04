/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_VIEWTRANSFORMERID_H
#define __BOI_VIEWTRANSFORMERID_H


#define BOI_VTID(id) (BOI::ViewTransformerId_##id)


namespace BOI {


enum ViewTransformerId
{
    ViewTransformerId_Invalid = 0,
    ViewTransformerId_Scroll,
    ViewTransformerId_Zoom,
    ViewTransformerId_KineticScroll,
    ViewTransformerId_KineticZoom,

    NumViewTransformerIds
};


} // namespace BOI


#endif //__BOI_VIEWTRANSFORMERID_H

