/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_VIEWTRANSFORMER_H
#define __BOI_VIEWTRANSFORMER_H


#include <QtGlobal>


namespace BOI {


class View;


class ViewTransformer
{
    public:
        ViewTransformer();

        virtual bool Initialize(int version, qreal* pArgs);

        virtual bool Run();
        virtual void Reset();

    protected:
        void Translate(qreal x, qreal y);
        void Scale(qreal scaleFactor);

        qreal UpdateInterval();

    private:
        friend class TransformManager;
        View* m_pView;

        qreal m_updateInterval;

        int m_version;
        int m_instanceId;

        ViewTransformer* m_pNext;
};


inline qreal ViewTransformer::UpdateInterval()
{
    return m_updateInterval;
}


} // namespace BOI


#endif //__BOI_VIEWTRANSFORMER_H

