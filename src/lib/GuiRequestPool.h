/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_GUIREQUESTPOOL_H
#define __BOI_GUIREQUESTPOOL_H


namespace BOI {


class GuiRequest;


class GuiRequestPool
{
    public:
        GuiRequestPool(int initialSize=17);
        ~GuiRequestPool();

        GuiRequest* Get();
        void Put(GuiRequest* pRequest);
        void Put(GuiRequest* pHead, GuiRequest* pTail);

    private:
        GuiRequest* m_pHead;
};


} // namespace BOI


#endif //__BOI_GUIREQUESTPOOL_H

