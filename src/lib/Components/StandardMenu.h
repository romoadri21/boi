/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_STANDARDMENU_H
#define __BOI_STANDARDMENU_H


#include <QPen>
#include <QFont>
#include <QList>
#include <QRectF>
#include <QBrush>
#include <QStringList>
#include <QFontMetricsF>


#define BOI_STANDARD_MENU_NUM_FILLS 4


class QImage;
class QPainter;


namespace BOI {


class StandardMenu
{
    public:
        StandardMenu();
        ~StandardMenu();

        void SetFont(const QFont& font);

        void SetXPadding(int padding);
        void SetYPadding(int padding);

        void SetTextPen(const QPen& pen);
        void SetBorderPen(const QPen& pen);

        void SetFill(int index, const QBrush& brush);

        void SetItems(const QStringList& items);
        void SetSections(const QList<int>& sections);

        void Update();

        const QRectF& BoundingRect() const;

        void Draw(QPainter* painter, const QRectF& rect);

        int IndexAt(const QPointF& point);

    private:
        QRectF m_boundingRect;

        QFont m_font;
        QFontMetricsF m_fontMetrics;

        int m_xPadding;
        int m_yPadding;

        QPen m_textPen;
        QPen m_borderPen;

        QBrush m_fills[BOI_STANDARD_MENU_NUM_FILLS];

        int m_singleCellHeight;

        QStringList m_items;
        QList<int> m_sections;

        int m_numCols;
        int m_numRows;
        int m_numItems;

        int* m_pColWidths;

        QImage* m_pImage;
};


} // namespace BOI


#endif //__BOI_STANDARDMENU_H

