/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QSizeF>
#include <QPainter>
#include <QVariant>
#include <QStyleOptionGraphicsItem>
#include "CSI.h"
#include "StandardDataTypes.h"
#include "StandardComponents.h"
#include "Components/BasicComponentDrawData.h"
#include "Components/WindowSizeComponent.h"


namespace BOI {


BOI_BEGIN_RECEIVERS(WindowSizeComponent)
BOI_END_RECEIVERS(WindowSizeComponent)


BOI_BEGIN_EMITTERS(WindowSizeComponent)
    BOI_DECLARE_EMITTER("{8d001b1b-c057-44c8-a4a6-aab8e9da1916}")
    BOI_DECLARE_EMITTER("{24a74700-21ae-4e99-8e95-366901620655}")
    BOI_DECLARE_EMITTER("{64078be7-c9eb-4851-bf29-5f2484a81401}")
BOI_END_EMITTERS(WindowSizeComponent)


BOI_BEGIN_FUNCSETS(WindowSizeComponent)
BOI_END_FUNCSETS(WindowSizeComponent)


BOI_BEGIN_CALLERS(WindowSizeComponent)
BOI_END_CALLERS(WindowSizeComponent)


WindowSizeComponent::WindowSizeComponent(const BasicComponentDrawData* pDrawData)
    : Component(BOI_STD_C(WindowSize)),
      m_sizeRef(),
      m_widthRef(),
      m_heightRef(),
      m_pDrawData(pDrawData)
{
}


bool WindowSizeComponent::Initialize()
{
    m_sizeRef   = SI()->NewData(BOI_STD_D(Size));
    m_widthRef  = SI()->NewData(BOI_STD_D(Float));
    m_heightRef = SI()->NewData(BOI_STD_D(Float));

    QVariant value;
    SI()->RegisterStateListener(StateId_WindowSize, this, value);
    SetSize(value.toSizeF());

    SetBoundingRect(m_pDrawData->BoundingRect());

    return true;
}


void WindowSizeComponent::Destroy()
{
    SI()->UnregisterStateListener(StateId_WindowSize, this);
}


void WindowSizeComponent::HandleStateChanged(StateId stateId, DRef& dref)
{
    Q_UNUSED(stateId);

    const QSizeF* pSize = dref.GetReadInstance<QSizeF>();
    SetSize(*pSize);
}


void WindowSizeComponent::HandleEmitterConnected(int emitter,
                                                 int componentId)
{
    Q_UNUSED(componentId);

    if (EmitterHasNew(emitter))
    {
        if (emitter == Emitter_Size)
        {
            EmitToNew(Emitter_Size, m_sizeRef);
        }
        else if (emitter == Emitter_Width)
        {
            EmitToNew(Emitter_Width, m_widthRef);
        }
        else if (emitter == Emitter_Height)
        {
            EmitToNew(Emitter_Height, m_heightRef);
        }
    }
}


void WindowSizeComponent::SetSize(const QSizeF& size)
{
    qreal width  = size.width();
    qreal height = size.height();

    QSizeF* pSize = m_sizeRef.GetWriteInstance<QSizeF>();
    pSize->setWidth(width);
    pSize->setHeight(height);

    if (EmitterConnected(Emitter_Size))
    {
        Emit(Emitter_Size, m_sizeRef);
    }


    float* pWidth = m_widthRef.GetWriteInstance<float>();
    *pWidth = width;

    if (EmitterConnected(Emitter_Width))
    {
        Emit(Emitter_Width, m_widthRef);
    }


    float* pHeight = m_heightRef.GetWriteInstance<float>();
    *pHeight = height;

    if (EmitterConnected(Emitter_Height))
    {
        Emit(Emitter_Height, m_heightRef);
    }
}


void WindowSizeComponent::Draw(QPainter* pPainter,
                               const QStyleOptionGraphicsItem* pOption)
{
    pPainter->drawImage(pOption->exposedRect,
                        m_pDrawData->Image(),
                        pOption->exposedRect);
}


} // namespace BOI

