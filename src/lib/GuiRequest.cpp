/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "memory.h"
#include "GuiRequest.h"


namespace BOI {


GuiRequest::GuiRequest()
    : cref(),
      cref2(),
      pNext(NULL)
{
}


GuiRequest::GuiRequest(const GuiRequest& request)
{
    /*
     * Note, the CRefs must be copied before the memcpy
     * otherwise the previous CRefs could have incorrect
     * data and not perform a Reset() when it should have.
     */
    cref  = request.cref;
    cref2 = request.cref2;

    memcpy(this, &request, sizeof(GuiRequest));
}


GuiRequest& GuiRequest::operator=(const GuiRequest& request)
{
    if (this != &request)
    {
        /*
         * Note, the CRefs must be copied before the memcpy
         * otherwise the previous CRefs could have incorrect
         * data and not perform a Reset() when it should have.
         */
        cref  = request.cref;
        cref2 = request.cref2;

        memcpy(this, &request, sizeof(GuiRequest));
    }

    return *this;
}


} // namespace BOI

