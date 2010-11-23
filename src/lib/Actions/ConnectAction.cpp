/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QSizeF>
#include <QPointF>
#include <QVariant>
#include "ASI.h"
#include "CRefList.h"
#include "ActionArgs.h"
#include "Description.h"
#include "StandardActions.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Events/TouchEvent.h"
#include "Actions/ConnectAction.h"


namespace BOI {


ConnectAction::ConnectAction()
    : Action(BOI_STD_A(Connect)),
      m_pageLoaded(false)
{
}


ActionCommand ConnectAction::Start(ASI* pSI, const ActionArgs* pArgs)
{
    Q_UNUSED(pArgs);

    m_getSource = true;
    m_getTarget = true;
    m_getEmitter = true;
    m_getReceiver = true;

    if ((pArgs != NULL) &&
        (pArgs->Contains("Type")))
    {
        int type = pArgs->Value<int>("Type");

        if (type & ProfileType_Emitters)
        {
            m_source = pSI->ActiveComponent();

            if (m_source.IsValid())
            {
                if (GetProfile(pSI, m_source, ProfileType_Emitters))
                {
                    m_getSource = false;

                    ShowList(pSI);

                    return BOI_AC_CONTINUE;
                }
            }
        }
        else if (type & ProfileType_Receivers)
        {
            m_target = pSI->ActiveComponent();

            if (m_target.IsValid())
            {
                if (GetProfile(pSI, m_target, ProfileType_Receivers))
                {
                    m_getTarget = false;

                    ShowList(pSI);

                    return BOI_AC_CONTINUE;
                }
            }
        }
    }

    return BOI_AC_STOP;
}


void ConnectAction::Stop(ASI* pSI)
{
    Q_UNUSED(pSI);

    if (m_browserComponent.IsValid())
    {
        pSI->SetVisible(m_browserComponent, false);
    }

    m_source.Reset();
    m_target.Reset();
}


void ConnectAction::Destroy()
{
    m_browserComponent.Reset();
}


bool ConnectAction::AcceptTouchStream()
{
    return m_acceptTouchStream;
}


ActionCommand ConnectAction::HandleTouchEvent(ASI* pSI, TouchEvent* pEvent)
{
    int eventType = pEvent->type;

    if (eventType == TouchEvent::Type_Press)
    {
        QPoint point(pEvent->x, pEvent->y);

        int sceneLayers = SceneLayerId_Main |
                          SceneLayerId_Overlay |
                          SceneLayerId_Underlay;

        CRefList crefs = pSI->ComponentsAtViewPoint(point, sceneLayers);

        if (crefs.Count() > 0)
        {
            if (m_getSource)
            {
                m_source = crefs.Value(0);
                m_getSource = false;

                if (GetProfile(pSI, m_source, ProfileType_Emitters))
                {
                    ShowList(pSI);
                }
                else
                {
                    return BOI_AC_STOP;
                }
            }
            else if (m_getTarget)
            {
                m_target = crefs.Value(0);
                m_getTarget = false;

                if (GetProfile(pSI, m_target, ProfileType_Receivers))
                {
                    ShowList(pSI);
                }
                else
                {
                    return BOI_AC_STOP;
                }
            }
        }

        m_acceptTouchStream = false;
    }
    else if (eventType == TouchEvent::Type_Release) 
    {
        m_acceptTouchStream = (m_getSource || m_getTarget);
    }

    return BOI_AC_CONTINUE;
}


void ConnectAction::ShowList(ASI* pSI)
{
    if (!m_browserComponent.IsValid() ||
         m_browserComponent.IsDestroyed())
    {
        m_browserComponent = pSI->NewComponent(BOI_STD_C(Browser),
                                               SceneLayerId_System);

        /*
         * Set the Action type id so that
         * Update gets called correctly.
         */

        int funcSet = pSI->GetFuncSet(m_browserComponent,
                                      "{790e6f3f-4433-4490-a141-a4cb4433b0e7}");
        DRef dref = pSI->NewData(BOI_STD_D(Int));
        *dref.GetWriteInstance<int>() = Type();
        pSI->CallFunc(m_browserComponent, funcSet, 0, dref);

        /*
         *  Set the preferred width of the browser page
         *  to a value smaller than the actual width of
         *  the html page to ensure that there is no
         *  extra 'body' space on the sides of the page.
         */

        int receiver = pSI->GetReceiver(m_browserComponent,
                                        "{f0c430fa-ade2-41c8-bba8-f9c6eb03b183}");
        dref = pSI->NewData(BOI_STD_D(Int));
        *dref.GetWriteInstance<int>() = 10;
        pSI->EmitTo(m_browserComponent, receiver, dref);

        /*
         * Load the connector select page into the browser.
         */

        receiver = pSI->GetReceiver(m_browserComponent,
                                    "{7473aebc-56e3-4dc4-994e-da426f45866c}");

        dref = pSI->NewData(BOI_STD_D(String));
        *dref.GetWriteInstance<QString>() = "qrc:///BOI/ConnectorSelect.html";
        pSI->EmitTo(m_browserComponent, receiver, dref);

        m_pageLoaded = false;


        m_evaluateJSReceiver = pSI->GetReceiver(m_browserComponent,
                                                "{84235b00-1b96-43bf-907b-240dfc74e6b0}");
    }

    /*
     * Center the component in the view.
     */
    QVariant var;
    pSI->GetState(StateId_WindowSize, var);
    QSizeF size = var.toSizeF();
    qreal left = (size.width() / 2) - 250;
    pSI->SetPosition(m_browserComponent, QPointF(left, 100));

    if (m_pageLoaded)
    {
        UpdatePage(pSI);
        pSI->SetVisible(m_browserComponent, true);
    }

    m_acceptTouchStream = false;
}


ActionCommand ConnectAction::Update(ASI* pSI, const ActionArgs* pArgs)
{
    if (pArgs == NULL) return BOI_AC_CONTINUE;

    bool stop = false;

    if (pArgs->Contains("PageLoaded"))
    {
        UpdatePage(pSI);
        pSI->SetVisible(m_browserComponent, true);

        m_pageLoaded = true;
    }
    else if (pArgs->Contains("Index"))
    {
        int index = pArgs->Value<int>("Index");

        if (m_getEmitter && !m_getSource)
        {
            m_emitter = m_uuids.at(index);
            m_getEmitter = false;
        }
        else if (m_getReceiver && !m_getTarget)
        {
            m_receiver = m_uuids.at(index);
            m_getReceiver = false;
        }

        if (!m_getEmitter && !m_getReceiver)
        {
            int emitter = pSI->GetEmitter(m_source, m_emitter);
            int receiver = pSI->GetReceiver(m_target, m_receiver);
            pSI->ConnectToReceiver(m_source, emitter, m_target, receiver);

            stop = true;
        }

        pSI->SetVisible(m_browserComponent, false);
        m_acceptTouchStream = true;
    }

    return (stop) ?
           (BOI_AC_STOP) :
           (BOI_AC_CONTINUE);
}


void ConnectAction::UpdatePage(ASI* pSI)
{
    QString script;

    if (!m_getSource && !m_getTarget)
    {
        script = "SetHeader('Select a connection point...');";
    }
    else
    {
        script = "SetHeader('Select a connection point and then "
                 "select a component to connect to...');";
    }

    script += "SetItems([";

    for (int i=0; i < m_numDescriptions; i++)
    {
        const Description* pDescription = m_descriptions[i];

        if (pDescription == NULL)
        {
            script += "['No Title Available', 'No Description Available']";
        }
        else
        {
            script += "['";
            script += pDescription->Title();
            script += "','";
            script += pDescription->Body();
            script += "']";
        }

        if (i < (m_numDescriptions - 1))
        {
            script += ',';
        }
    }

    script += "]);";

    DRef dref = pSI->NewData(BOI_STD_D(String));
    *dref.GetWriteInstance<QString>() = script;
    pSI->EmitTo(m_browserComponent, m_evaluateJSReceiver, dref);
}


bool ConnectAction::GetProfile(ASI* pSI, CRef& cref, ProfileType type)
{
    m_uuids = pSI->GetProfile(cref, type);
    m_numDescriptions = m_uuids.size();

    if (m_numDescriptions > 0)
    {
        for (int i=0; i < m_numDescriptions; i++)
        {
            m_descriptions[i] = pSI->GetDescription(m_uuids.at(i));
        }

        return true;
    }

    return false;
}


} // namespace BOI

