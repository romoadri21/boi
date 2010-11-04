/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_WINDOWSIZECOMPONENT_H
#define __BOI_WINDOWSIZECOMPONENT_H


#include "DRef.h"
#include "StandardEmitters.h"
#include "Component.h"


class QSizeF;


namespace BOI {


class BasicComponentDrawData;


class WindowSizeComponent
    : public Component
{
    BOI_INTERFACE_DEFINITION();

    public:
        WindowSizeComponent(const BasicComponentDrawData* pDrawData);

    protected:
        virtual bool Initialize();
        virtual void Destroy();

        virtual void HandleStateChanged(StateId stateId, DRef& dref);

        virtual void HandleEmitterConnected(int emitter,
                                            int componentId);

        virtual void Draw(QPainter* pPainter,
                          const QStyleOptionGraphicsItem* pOption);

    private:
        enum
        {
            Emitter_Size = StandardEmitters::NumEmitters,
            Emitter_Width,
            Emitter_Height
        };

    private:
        void SetSize(const QSizeF& size);

    private:
        DRef m_sizeRef;
        DRef m_widthRef;
        DRef m_heightRef;

        const BasicComponentDrawData* m_pDrawData;
};


} // namespace BOI


#endif //__BOI_WINDOWSIZECOMPONENT_H

