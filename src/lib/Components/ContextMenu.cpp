/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QImage>
#include <QPainter>
#include "math.h"
#include "ContextMenu.h"


namespace BOI {


ContextMenu::ContextMenu()
    : m_boundingRect(),
      m_font("sans", 11, QFont::Normal),
      m_fontMetrics(m_font),
      m_xPadding(7),
      m_yPadding(7),
      m_textPen(QColor(0, 0, 0)),
      m_borderPen(QColor(215, 215, 215)),
      m_fills(),
      m_leftOriented(true),
      m_singleCellHeight(0),
      m_items(),
      m_sections(),
      m_numCols(0),
      m_numRows(0),
      m_numItems(0),
      m_numEmptyCells(0),
      m_numExtraCells(0),
      m_pColWidths(NULL),
      m_pImage(NULL)
{
    m_sections.append(-1);

    m_fills[0] = QBrush(qRgb(250, 250, 250));
    m_fills[1] = QBrush(qRgb(235, 235, 235));
    m_fills[2] = QBrush(qRgb(215, 215, 255));
    m_fills[3] = QBrush(qRgb(215, 255, 215));
}


ContextMenu::~ContextMenu()
{
    delete m_pImage;
    delete m_pColWidths;
}


void ContextMenu::SetFont(const QFont& font)
{
    m_font = font;
    m_fontMetrics = QFontMetricsF(m_font);
}


void ContextMenu::SetXPadding(int padding)
{
    m_xPadding = padding;
}


void ContextMenu::SetYPadding(int padding)
{
    m_yPadding = padding;
}


void ContextMenu::SetTextPen(const QPen& pen)
{
    m_textPen = pen;
}


void ContextMenu::SetBorderPen(const QPen& pen)
{
    m_borderPen = pen;
}


void ContextMenu::SetFill(int index, const QBrush& brush)
{
    index = index % BOI_CONTEXT_MENU_NUM_FILLS;
    m_fills[index] = brush;
}


void ContextMenu::SetOrientation(bool leftOriented)
{
    m_leftOriented = leftOriented;
}


void ContextMenu::SetItems(const QStringList& items)
{
    m_items = items;
}


void ContextMenu::SetSections(const QList<int>& sections)
{
    m_sections = sections;
}


void ContextMenu::Update()
{
    m_numItems = m_items.size();

    if (m_numItems == 0)
    {
        /*
         * Invalidate the bounding rect.
         */
        m_boundingRect.setWidth(0);
        m_boundingRect.setHeight(0);

        return;
    }

    /*
     * Compute the dimensions for the table.
     */

    double idealNumCols = sqrt((double)m_numItems);
    double idealNumRows = idealNumCols;

    m_numCols = qRound(idealNumCols);
    m_numRows = ceil(idealNumRows);

    m_numEmptyCells = (m_numCols * m_numRows) - m_numItems;
    m_numExtraCells = (m_numItems != 1) ?
                      (m_numCols - m_numEmptyCells) : 0;

    /*
     * Get the total height, total width and the width
     * for all the columns.
     */

    if (m_pColWidths != NULL) delete m_pColWidths;
    m_pColWidths = new int[m_numCols];

    m_singleCellHeight = m_fontMetrics.height() + (m_yPadding * 2);
    int totalHeight = m_numRows * m_singleCellHeight;

    QRectF textBounds;

    int totalWidth = 0;
    int currentCol = 0;
    int currentRow = 0;
    int colWidth = 0;

    for (int i=0; i < m_numItems; i++)
    {
        textBounds = m_fontMetrics.boundingRect(m_items.at(i));
        if (textBounds.width() > colWidth)
        {
            colWidth = textBounds.width();
        }

        currentRow++;

        if (currentRow >= (m_numRows - 1))
        {
            bool doExtra = (m_leftOriented) ?
                           (currentCol < m_numExtraCells) :
                           ((currentCol >= m_numEmptyCells) && (m_numItems > 1));

            if (doExtra)
            {
                textBounds = m_fontMetrics.boundingRect(m_items.at(++i));

                if (textBounds.width() > colWidth)
                {
                    colWidth = textBounds.width();
                }
            }

            m_pColWidths[currentCol] = colWidth + (m_xPadding * 2);
            totalWidth += m_pColWidths[currentCol];

            currentRow = 0;
            colWidth = 0;
            currentCol++;
        }
    }


    totalWidth++;
    totalHeight++;

    m_boundingRect.setRect(0, 0, totalWidth, totalHeight);

    /*
     * Create a new image if the current image is NULL
     * or if the current image width and height do not
     * contain the new width and height.
     */

    if (m_pImage != NULL)
    {
        if ((m_pImage->width() < totalWidth) ||
            (m_pImage->height() < totalHeight))
        {
            delete m_pImage;
            m_pImage = new QImage(totalWidth,
                                  totalHeight,
                                  QImage::Format_ARGB32);
        }
    }
    else
    {
        m_pImage = new QImage(totalWidth,
                              totalHeight,
                              QImage::Format_ARGB32);
    }

    m_pImage->fill(0);


    /*
     * Draw the menu items.
     */

    QPainter painter(m_pImage);
    painter.setFont(m_font);

    int fillIndex = 0;
    int sectionIndex = 0;

    currentCol = 0;
    currentRow = 0;

    QRectF boundingRect(0, 0, m_pColWidths[0], m_singleCellHeight);

    for (int i=0; i < m_numItems; i++)
    {
        if (i == m_sections[sectionIndex])
        {
            sectionIndex++;
            fillIndex = sectionIndex % BOI_CONTEXT_MENU_NUM_FILLS;
        }

        painter.setPen(m_borderPen);
        painter.setBrush(m_fills[fillIndex]);
        painter.drawRect(boundingRect);

        painter.setPen(m_textPen);
        painter.drawText(boundingRect, Qt::AlignCenter, m_items.at(i));

        boundingRect.translate(0, m_singleCellHeight);
        currentRow++;

        if (currentRow >= (m_numRows - 1))
        {
            bool doExtra = (m_leftOriented) ?
                           (currentCol < m_numExtraCells) :
                           ((currentCol >= m_numEmptyCells) && (m_numItems > 1));

            if (doExtra)
            {
                i++;

                if (i == m_sections[sectionIndex])
                {
                    sectionIndex++;
                    fillIndex = sectionIndex % BOI_CONTEXT_MENU_NUM_FILLS;
                }

                painter.setPen(m_borderPen);
                painter.setBrush(m_fills[fillIndex]);
                painter.drawRect(boundingRect);

                painter.setPen(m_textPen);
                painter.drawText(boundingRect, Qt::AlignCenter, m_items.at(i));
            }

            boundingRect.translate(m_pColWidths[currentCol], 0);
            boundingRect.moveTop(0);

            currentRow = 0;
            currentCol++;

            if (currentCol < m_numCols)
            {
                boundingRect.setWidth(m_pColWidths[currentCol]);
            }
        }
    }
}


const QRectF& ContextMenu::BoundingRect() const
{
    return m_boundingRect;
}


void ContextMenu::Draw(QPainter* painter, const QRectF& rect)
{
    if (m_boundingRect.isValid())
    {
        QRectF commonRect = m_boundingRect.intersected(rect);

        if (!commonRect.isEmpty())
        {
            painter->drawImage(commonRect, *m_pImage, commonRect);
        }
    }
}


int ContextMenu::IndexAt(const QPointF& point)
{
    if (m_numItems > 0)
    {
        if (m_boundingRect.isValid() &&
            m_boundingRect.contains(point))
        {
            if (m_numItems == 1) return 0;

            qreal x = point.x();
            qreal y = point.y();

            int currentWidth = 0;

            for (int i=0; i < m_numCols; i++)
            {
                currentWidth += m_pColWidths[i];

                if (x < currentWidth)
                {
                    int index = 0;
                    int row = floor(y / m_singleCellHeight);

                    if (m_leftOriented)
                    {
                        index = (m_numRows - 1) * i;
                        index += row;

                        if (i >= m_numExtraCells)
                        {
                            if (row == (m_numRows-1)) return -1;

                            index += m_numExtraCells;
                        }
                        else
                        {
                            index += i;
                        }
                    }
                    else
                    {
                        index = (m_numRows * i) + row;

                        if (i >= m_numEmptyCells)
                        {
                            index -= m_numEmptyCells;
                        }
                        else
                        {
                            if (row == (m_numRows-1)) return -1;

                            index -= i;
                        }
                    }

                    if (index < m_numItems)
                    {
                        return index;
                    }
                }
            }
        }
    }

    return -1;
}


} // namespace BOI

