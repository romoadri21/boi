/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QPen>
#include <QFont>
#include <QFile>
#include <QRectF>
#include <QSizeF>
#include <QBrush>
#include <QColor>
#include <QDataStream>
#include <QReadWriteLock>
#include "ISI.h"
#include "StandardDataTypes.h"
#include "Events/ResizeEvent.h"
#include "State.h"


#define BOI_STATE_MAGIC_NUMBER    0xA749BC01
#define BOI_STATE_VERSION_1       0x00000001
#define BOI_STATE_CURRENT_VERSION BOI_STATE_VERSION_1


namespace BOI {


State::State()
    : m_activeComponent(),
      m_keyEventHandler(),
      m_selection()
{
    QReadWriteLock* pLock = new QReadWriteLock();

    m_data[StateId_InnerViewXMargin].value = 0.1;
    m_data[StateId_InnerViewXMargin].pLock = pLock;

    m_data[StateId_InnerViewYMargin].value = 0.1;
    m_data[StateId_InnerViewYMargin].pLock = pLock;

    m_data[StateId_InnerViewRect].value = QRectF(0, 0, 1, 1);
    m_data[StateId_InnerViewRect].pLock = pLock;

    m_data[StateId_WindowSize].value = QSizeF(1.0, 1.0);
    m_data[StateId_WindowSize].pLock = pLock;


    m_data[StateId_FgColor].value = Qt::black;
    m_data[StateId_FgColor].pLock = new QReadWriteLock();

    m_data[StateId_BgColor].value = Qt::white;
    m_data[StateId_BgColor].pLock = new QReadWriteLock();

    m_data[StateId_Brush].value = QBrush(Qt::white);
    m_data[StateId_Brush].pLock = new QReadWriteLock();

    m_data[StateId_Font].value = QFont("Arial", 12);
    m_data[StateId_Font].pLock = new QReadWriteLock();

    m_data[StateId_Pen].value = QPen(Qt::black, 1);
    m_data[StateId_Pen].pLock = new QReadWriteLock();

    m_data[StateId_ScrollVelocity].value = 500.0;
    m_data[StateId_ScrollVelocity].pLock = new QReadWriteLock();

    m_data[StateId_KineticScrollAcceleration].value = -8000.0;
    m_data[StateId_KineticScrollAcceleration].pLock = new QReadWriteLock();

    m_data[StateId_KineticScrollActivateVelocity].value = 1400.0;
    m_data[StateId_KineticScrollActivateVelocity].pLock = new QReadWriteLock();


    for (int i=0; i < NumStateIds; i++)
    {
        m_data[i].pListeners = NULL;
    }
}


State::~State()
{
    UnregisterAllListeners();

    delete m_data[StateId_WindowSize].pLock;
    delete m_data[StateId_FgColor].pLock;
    delete m_data[StateId_BgColor].pLock;
    delete m_data[StateId_Brush].pLock;
    delete m_data[StateId_Font].pLock;
    delete m_data[StateId_Pen].pLock;
    delete m_data[StateId_ScrollVelocity].pLock;
    delete m_data[StateId_KineticScrollAcceleration].pLock;
    delete m_data[StateId_KineticScrollActivateVelocity].pLock;
}


void State::SetISI(ISI* pISI)
{
    m_pISI = pISI;
}


bool State::Load(const QString& path)
{
    if (!QFile::exists(path)) return false;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QDataStream in(&file);

    quint32 magicNumber;
    in >> magicNumber;
    if (magicNumber != BOI_STATE_MAGIC_NUMBER) return false;

    qint32 version;
    in >> version;

    if (version == BOI_STATE_VERSION_1)
    {
        in.setVersion(QDataStream::Qt_4_6);

        for (int i=0; i < NumStateIds; i++)
        {
            in >> m_data[i].value;
        }

        return true;
    }

    return false;
}


bool State::Save(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) return false;

    QDataStream out(&file);

    out << (quint32)BOI_STATE_MAGIC_NUMBER;
    out << (qint32) BOI_STATE_CURRENT_VERSION;

    out.setVersion(QDataStream::Qt_4_6);

    for (int i=0; i < NumStateIds; i++)
    {
        out << m_data[i].value;
    }

    file.close();
    return true;
}


bool State::Get(StateId stateId, QVariant& value)
{
    if ((stateId < 0) || (stateId >= NumStateIds)) return false;

    StateData* pStateData = &m_data[stateId];
    pStateData->pLock->lockForRead();

    value = pStateData->value;

    pStateData->pLock->unlock();
    return true;
}


bool State::Set(StateId stateId, const QVariant& value)
{
    switch (stateId)
    {
        case StateId_InnerViewXMargin:
        case StateId_InnerViewYMargin:
            if (value.type() != (int)QMetaType::Double)
            {
                if (!value.canConvert<double>()) return false;
            }

            UpdateViewData(stateId, value);
            return true;

        case StateId_FgColor:
        case StateId_BgColor:
            if (value.type() != (int)QMetaType::QColor)
            {
                if (!value.canConvert<QColor>()) return false;
            }
            break;

        case StateId_Brush:
            if (value.type() != (int)QMetaType::QBrush)
            {
                if (!value.canConvert<QBrush>()) return false;
            }
            break;

        case StateId_Font:
            if (value.type() != (int)QMetaType::QFont)
            {
                if (!value.canConvert<QFont>()) return false;
            }
            break;

        case StateId_Pen:
            if (value.type() != (int)QMetaType::QPen)
            {
                if (!value.canConvert<QPen>()) return false;
            }
            break;

        case StateId_ScrollVelocity:
        case StateId_KineticScrollAcceleration:
        case StateId_KineticScrollActivateVelocity:
            if (value.type() != (int)QMetaType::Double)
            {
                if (!value.canConvert<double>()) return false;
            }
            break;

        default:
            return false;
    }

    SetData(stateId, value);

    return true;
}


void State::RegisterListener(StateId stateId, CRef cref, QVariant& value)
{
    if ((stateId < 0) || (stateId >= NumStateIds)) return;

    bool alreadyRegistered = false;

    StateData* pStateData = &m_data[stateId];
    pStateData->pLock->lockForWrite();

    Listener* pListener = pStateData->pListeners;
    while (pListener != NULL)
    {
        if (pListener->cref == cref)
        {
            alreadyRegistered = true;
            break;
        }

        pListener = pListener->pNext;
    }

    if (!alreadyRegistered)
    {
        pListener = new Listener;
        pListener->cref = cref;
        pListener->pNext = pStateData->pListeners;

        pStateData->pListeners = pListener;
    }

    value = pStateData->value;

    pStateData->pLock->unlock();
}


void State::UnregisterListener(StateId stateId, CRef cref)
{
    if ((stateId < 0) || (stateId >= NumStateIds)) return;

    StateData* pStateData = &m_data[stateId];
    pStateData->pLock->lockForWrite();

    Listener* pListener = pStateData->pListeners;

    if (pListener != NULL)
    {
        if (pListener->cref == cref)
        {
            pStateData->pListeners = pListener->pNext;
        }
        else
        {
            Listener* pPrev = pListener;
            pListener = pListener->pNext;

            while (pListener != NULL)
            {
                if (pListener->cref == cref)
                {
                    pPrev->pNext = pListener->pNext;
                    break;
                }

                pPrev = pListener;
                pListener = pListener->pNext;
            }
        }
    }

    pStateData->pLock->unlock();
    delete pListener;
}


void State::UnregisterAllListeners()
{
    for (int i=0; i < NumStateIds; i++)
    {
        Listener* pListener = m_data[i].pListeners;

        while (pListener != NULL)
        {
            Listener* pNext = pListener->pNext;
            delete pListener;
            pListener = pNext;
        }

        m_data[i].pListeners = NULL;
    }
}


void State::HandleResizeEvent(ResizeEvent* pEvent)
{
    QSizeF size(pEvent->width, pEvent->height);
    QVariant value = size;

    UpdateViewData(StateId_WindowSize, value);
}


void State::SetData(StateId stateId, const QVariant& value)
{
    m_data[stateId].pLock->lockForWrite();
    m_data[stateId].value = value;

    if (m_data[stateId].pListeners != NULL)
    {
        DRef dref;

        switch (stateId)
        {
            case StateId_FgColor:
            case StateId_BgColor:
                {
                    dref = m_pISI->NewData(BOI_STD_D(Color));
                    *dref.GetWriteInstance<QColor>() = value.value<QColor>();
                }
                break;

            case StateId_Brush:
                {
                    dref = m_pISI->NewData(BOI_STD_D(Brush));
                    *dref.GetWriteInstance<QBrush>() = value.value<QBrush>();
                }
                break;

            case StateId_Font:
                {
                    dref = m_pISI->NewData(BOI_STD_D(Font));
                    *dref.GetWriteInstance<QFont>() = value.value<QFont>();
                }
                break;

            case StateId_Pen:
                {
                    dref = m_pISI->NewData(BOI_STD_D(Pen));
                    *dref.GetWriteInstance<QPen>() = value.value<QPen>();
                }
                break;

            case StateId_ScrollVelocity:
            case StateId_KineticScrollAcceleration:
            case StateId_KineticScrollActivateVelocity:
                {
                    dref = m_pISI->NewData(BOI_STD_D(Double));
                    *dref.GetWriteInstance<double>() = value.toDouble();
                }
                break;

            default:
                return;
        }

        PostToListeners(stateId, dref);
    }

    m_data[stateId].pLock->unlock();
}


void State::UpdateViewData(StateId stateId, const QVariant& value)
{
    m_data[stateId].pLock->lockForWrite();
    m_data[stateId].value = value;

    /*
     * Update the InnerViewRect.
     */

    double xMargin = m_data[StateId_InnerViewXMargin].value.toDouble();
    double yMargin = m_data[StateId_InnerViewYMargin].value.toDouble();
    QSizeF size = m_data[StateId_WindowSize].value.toSizeF();

    xMargin = size.width() * xMargin;
    yMargin = size.height() * yMargin;

    QRectF innerViewRect(xMargin,
                         yMargin,
                         size.width()  - (xMargin * 2),
                         size.height() - (yMargin * 2));

    m_data[StateId_InnerViewRect].value = innerViewRect;


    if (m_data[StateId_InnerViewRect].pListeners != NULL)
    {
        DRef dref = m_pISI->NewData(BOI_STD_D(Rect));
        *dref.GetWriteInstance<QRectF>() = innerViewRect;

        PostToListeners(StateId_InnerViewRect, dref);
    }


    if (m_data[stateId].pListeners != NULL)
    {
        DRef dref;

        switch (stateId)
        {
            case StateId_InnerViewXMargin:
                {
                    dref = m_pISI->NewData(BOI_STD_D(Double));
                    *dref.GetWriteInstance<double>() = value.toDouble();
                }
                break;

            case StateId_InnerViewYMargin:
                {
                    dref = m_pISI->NewData(BOI_STD_D(Double));
                    *dref.GetWriteInstance<double>() = value.toDouble();
                }
                break;

            case StateId_WindowSize:
                {
                    dref = m_pISI->NewData(BOI_STD_D(Size));
                    *dref.GetWriteInstance<QSizeF>() = value.toSizeF();
                }
                break;

            default:
                break;
        }

        PostToListeners(stateId, dref);
    }

    m_data[stateId].pLock->unlock();
}


void State::PostToListeners(StateId stateId, DRef& dref)
{
    Listener* pListener = m_data[stateId].pListeners;
    while (pListener != NULL)
    {
        m_pISI->PostStateChangedTask(pListener->cref, stateId, dref);

        pListener = pListener->pNext;
    }
}


CRef State::ActiveComponent()
{
    return m_activeComponent;
}


void State::SetActiveComponent(CRef cref)
{
    m_activeComponent = cref;
}


CRef State::KeyEventHandler()
{
    return m_keyEventHandler;
}


void State::SetKeyEventHandler(CRef cref)
{
    m_keyEventHandler = cref;
}


CRefList State::Selection()
{
    return m_selection;
}


void State::SetSelection(CRefList crefList)
{
    m_selection = crefList;
}


void State::ResetCRefs()
{
    m_activeComponent.Reset();
    m_keyEventHandler.Reset();

    m_selection = CRefList();
}


} // namespace BOI

