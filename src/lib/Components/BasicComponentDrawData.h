/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_BASICCOMPONENTDRAWDATA_H
#define __BOI_BASICCOMPONENTDRAWDATA_H


#include <QRectF>


class QSizeF;
class QImage;


namespace BOI {


class BasicComponentDrawData
{
    public:
        BasicComponentDrawData();
        ~BasicComponentDrawData();

        void SetText(const QString& text);
        void SetSvg(const QString& path, const QSizeF& size);

        const QImage& Image() const;
        const QRectF& BoundingRect() const;

    private:
        QImage* m_pImage;
        QRectF  m_boundingRect;
};


inline const QImage& BasicComponentDrawData::Image() const
{
    return *m_pImage;
}


inline const QRectF& BasicComponentDrawData::BoundingRect() const
{
    return m_boundingRect;
}


} // namespace BOI


#endif //__BOI_BASICCOMPONENTDRAWDATA_H

