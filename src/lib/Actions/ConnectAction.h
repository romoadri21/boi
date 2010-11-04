/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_CONNECTACTION_H
#define __BOI_CONNECTACTION_H


#include <QStringList>
#include <QString>
#include "CRef.h"
#include "Action.h"
#include "ProfileType.h"


#define BOI_CONNECT_ACTION_MAX_DESCRIPTIONS 30


namespace BOI {


class Description;


class ConnectAction
    : public Action
{
    public:
        ConnectAction();

        ActionCommand Start(ASI* pSI, const ActionArgs* pArgs);

        void Stop(ASI* pSI);
        void Destroy();

        bool AcceptTouchStream();

        ActionCommand HandleTouchEvent(ASI* pSI, TouchEvent* pEvent);

        ActionCommand Update(ASI* pSI, const ActionArgs* pArgs);

    private:
        void ShowList(ASI* pSI);
        void UpdatePage(ASI* pSI);
        bool GetProfile(ASI* pSI, CRef& cref, ProfileType type);

    private:
        QString m_emitter;
        QString m_receiver;

        CRef m_source;
        CRef m_target;

        bool m_getSource;
        bool m_getTarget;
        bool m_getEmitter;
        bool m_getReceiver;

        bool m_acceptTouchStream;

        int  m_evaluateJSReceiver;
        CRef m_browserComponent;
        bool m_pageLoaded;

        QStringList m_uuids;

        int m_numDescriptions;
        const Description* m_descriptions[BOI_CONNECT_ACTION_MAX_DESCRIPTIONS];
};


} // namespace BOI


#endif //__BOI_CONNECTACTION_H

