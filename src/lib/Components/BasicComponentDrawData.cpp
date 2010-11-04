/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QPen>
#include <QFont>
#include <QImage>
#include <QSizeF>
#include <QPainter>
#include <QSvgRenderer>
#include <QFontMetricsF>
#include "Components/BasicComponentDrawData.h"


namespace BOI {


BasicComponentDrawData::BasicComponentDrawData()
    : m_pImage(NULL),
      m_boundingRect()
{
}


BasicComponentDrawData::~BasicComponentDrawData()
{
    delete m_pImage;
}


void BasicComponentDrawData::SetText(const QString& text)
{
    QFont font("arial", 13, QFont::Bold);
    QFontMetricsF fontMetrics(font);

    QRectF pageRect(0, 0, 1000, 1000);
    m_boundingRect = fontMetrics.boundingRect(pageRect,
                                              Qt::AlignCenter,
                                              text);

    /*
     * Move the origin to the top left and add the padding
     */

    m_boundingRect.moveTo(0, 0);

    float paddingX = fontMetrics.averageCharWidth();
    float paddingY = paddingX;
    m_boundingRect.translate(paddingX, paddingY);
    m_boundingRect.adjust(-paddingX, -paddingY, paddingX, paddingY);

    qreal penWidth = 2;
    QSize newSize = m_boundingRect.size().toSize();

    if (m_pImage != NULL)
    {
        if ((m_pImage->width()  < newSize.width()) ||
            (m_pImage->height() < newSize.height()))
        {
            delete m_pImage;
            m_pImage = new QImage(newSize, QImage::Format_ARGB32);
        }
    }
    else
    {
        m_pImage = new QImage(newSize, QImage::Format_ARGB32);
    }

    m_pImage->fill(0);

    QPainter painter(m_pImage);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setFont(font);
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, penWidth));

    painter.drawRoundedRect(m_boundingRect.adjusted(penWidth,
                                                    penWidth,
                                                    -penWidth,
                                                    -penWidth), 3, 3);

    painter.drawText(m_boundingRect, Qt::AlignCenter, text);
}


void BasicComponentDrawData::SetSvg(const QString& path, const QSizeF& size)
{
    QSvgRenderer renderer(path);
    QSize newSize = renderer.defaultSize();
    newSize.scale(size.toSize(), Qt::KeepAspectRatio);

    m_boundingRect.setRect(0, 0, newSize.width(), newSize.height());

    if (m_pImage != NULL)
    {
        if ((m_pImage->width()  < newSize.width()) ||
            (m_pImage->height() < newSize.height()))
        {
            delete m_pImage;
            m_pImage = new QImage(newSize, QImage::Format_ARGB32);
        }
    }
    else
    {
        m_pImage = new QImage(newSize, QImage::Format_ARGB32);
    }

    m_pImage->fill(0);

    QPainter painter(m_pImage);
    renderer.render(&painter, m_boundingRect);
}


} // namespace BOI

