/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_MENUACTION_H
#define __BOI_MENUACTION_H


#include "CRef.h"
#include "Action.h"


class QPointF;


namespace BOI {


class MenuAction
    : public Action
{
    public:
        MenuAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);
        void Destroy();

    protected:
        bool ShowMenu(ASI* pSI, const QPointF& centerPoint);

    private:
        CRef m_cref;
        int m_funcSet;
};


} // namespace BOI


#endif //__BOI_MENUACTION_H

