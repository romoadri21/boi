/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QImage>
#include <QPainter>
#include "math.h"
#include "StandardMenu.h"


namespace BOI {


StandardMenu::StandardMenu()
    : m_boundingRect(), 
      m_font("sans", 11, QFont::Normal),
      m_fontMetrics(m_font),
      m_xPadding(7),
      m_yPadding(7),
      m_textPen(QColor(0, 0, 0)),
      m_borderPen(QColor(215, 215, 215)),
      m_fills(),
      m_singleCellHeight(0),
      m_items(),
      m_sections(),
      m_numCols(0),
      m_numRows(0),
      m_numItems(0),
      m_pColWidths(NULL),
      m_pImage(NULL)
{
    m_sections.append(-1);

    m_fills[0] = QBrush(qRgb(250, 250, 250));
    m_fills[1] = QBrush(qRgb(235, 235, 235));
    m_fills[2] = QBrush(qRgb(215, 215, 255));
    m_fills[3] = QBrush(qRgb(215, 255, 215));
}


StandardMenu::~StandardMenu()
{
    delete m_pImage;
    delete[] m_pColWidths;
}


void StandardMenu::SetFont(const QFont& font)
{
    m_font = font;
    m_fontMetrics = QFontMetricsF(m_font);
}


void StandardMenu::SetXPadding(int padding)
{
    m_xPadding = padding;
}


void StandardMenu::SetYPadding(int padding)
{
    m_yPadding = padding;
}


void StandardMenu::SetTextPen(const QPen& pen)
{
    m_textPen = pen;
}


void StandardMenu::SetBorderPen(const QPen& pen)
{
    m_borderPen = pen;
}


void StandardMenu::SetFill(int index, const QBrush& brush)
{
    index = index % BOI_STANDARD_MENU_NUM_FILLS;
    m_fills[index] = brush;
}


void StandardMenu::SetItems(const QStringList& items)
{
    m_items = items;
}


void StandardMenu::SetSections(const QList<int>& sections)
{
    m_sections = sections;
}


void StandardMenu::Update()
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

    m_numCols = ceil(idealNumCols);
    m_numRows = qRound(idealNumRows);
    int numFullCols = floor(idealNumCols);

    /*
     * Get the total height, total width and the width
     * for all the columns.
     */

    if (m_pColWidths != NULL) delete[] m_pColWidths;
    m_pColWidths = new int[m_numCols];

    m_singleCellHeight = m_fontMetrics.height() + (m_yPadding * 2);
    int totalHeight = m_numRows * m_singleCellHeight;

    QRectF textBounds;

    int totalWidth = 0;
    int currentCol = 0;
    int currentRow = 0;
    int itemIndex = 0;
    int colWidth = 0;

    while (currentCol < numFullCols)
    {
        textBounds = m_fontMetrics.boundingRect(m_items.at(itemIndex));
        if (textBounds.width() > colWidth)
        {
            colWidth = textBounds.width();
        }

        currentRow++;

        if (currentRow == m_numRows)
        {
            m_pColWidths[currentCol] = colWidth + (m_xPadding * 2);
            totalWidth += m_pColWidths[currentCol];

            currentCol++;
            colWidth = 0;
            currentRow = 0;
        }

        itemIndex++;
    }

    if (itemIndex < m_numItems)
    {
        colWidth = 0;

        do
        {
            textBounds = m_fontMetrics.boundingRect(m_items.at(itemIndex));
            if (textBounds.width() > colWidth)
            {
                colWidth = textBounds.width();
            }

            itemIndex++;
        } while (itemIndex < m_numItems);

        m_pColWidths[currentCol] = colWidth + (m_xPadding * 2);
        totalWidth += m_pColWidths[currentCol];
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

    itemIndex = 0;
    currentCol = 0;
    currentRow = 0;

    int fillIndex = 0;
    int sectionIndex = 0;

    QRectF boundingRect(0, 0, m_pColWidths[0], m_singleCellHeight);

    while (currentCol < numFullCols)
    {
        if (itemIndex == m_sections[sectionIndex])
        {
            sectionIndex++;
            fillIndex = sectionIndex % BOI_STANDARD_MENU_NUM_FILLS;
        }

        painter.setPen(m_borderPen);
        painter.setBrush(m_fills[fillIndex]);
        painter.drawRect(boundingRect);

        painter.setPen(m_textPen);
        painter.drawText(boundingRect, Qt::AlignCenter, m_items.at(itemIndex));

        boundingRect.translate(0, m_singleCellHeight);
        currentRow++;

        if (currentRow == m_numRows)
        {
            boundingRect.translate(m_pColWidths[currentCol], 0);
            boundingRect.moveTop(0);

            currentCol++;
            currentRow = 0;

            boundingRect.setWidth(m_pColWidths[currentCol]);
        }

        itemIndex++;
    }

    if (itemIndex < m_numItems)
    {
        boundingRect.setWidth(m_pColWidths[currentCol]);

        do
        {
            if (itemIndex == m_sections[sectionIndex])
            {
                sectionIndex++;
                fillIndex = sectionIndex % BOI_STANDARD_MENU_NUM_FILLS;
            }

            painter.setPen(m_borderPen);
            painter.setBrush(m_fills[fillIndex]);
            painter.drawRect(boundingRect);

            painter.setPen(m_textPen);
            painter.drawText(boundingRect, Qt::AlignCenter, m_items.at(itemIndex));

            boundingRect.translate(0, m_singleCellHeight);

            itemIndex++;
        } while (itemIndex < m_numItems);
    }
}


const QRectF& StandardMenu::BoundingRect() const
{
    return m_boundingRect;
}


void StandardMenu::Draw(QPainter* painter, const QRectF& rect)
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


int StandardMenu::IndexAt(const QPointF& point)
{
    if (m_numItems > 0)
    {
        if (m_boundingRect.isValid() &&
            m_boundingRect.contains(point))
        {
            qreal x = point.x();
            qreal y = point.y();

            int currentWidth = 0;

            for (int i=0; i < m_numCols; i++)
            {
                currentWidth += m_pColWidths[i];

                if (x < currentWidth)
                {
                    int row = floor(y / m_singleCellHeight);
                    int index = (m_numRows * i) + row;

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

