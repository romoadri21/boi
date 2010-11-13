/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include <QImage>
#include <QPainter>
#include "math.h"
#include "RootMenu.h"


namespace BOI {


RootMenu::RootMenu()
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
      m_numCols(4),
      m_numRows(0),
      m_numItems(0),
      m_lastRowLeft(0),
      m_standardRowLeft(0),
      m_pColWidths(NULL),
      m_pImage(NULL)
{
    m_sections.append(-1);

    m_fills[0] = QBrush(qRgb(250, 250, 250));
    m_fills[1] = QBrush(qRgb(235, 235, 235));
    m_fills[2] = QBrush(qRgb(215, 215, 255));
    m_fills[3] = QBrush(qRgb(215, 255, 215));
}


RootMenu::~RootMenu()
{
    delete m_pImage;
    delete[] m_pColWidths;
}


void RootMenu::SetFont(const QFont& font)
{
    m_font = font;
    m_fontMetrics = QFontMetricsF(m_font);
}


void RootMenu::SetXPadding(int padding)
{
    m_xPadding = padding;
}


void RootMenu::SetYPadding(int padding)
{
    m_yPadding = padding;
}


void RootMenu::SetTextPen(const QPen& pen)
{
    m_textPen = pen;
}


void RootMenu::SetBorderPen(const QPen& pen)
{
    m_borderPen = pen;
}


void RootMenu::SetFill(int index, const QBrush& brush)
{
    index = index % BOI_ROOT_MENU_NUM_FILLS;
    m_fills[index] = brush;
}


void RootMenu::SetItems(const QStringList& items)
{
    m_items = items;
}


void RootMenu::SetSections(const QList<int>& sections)
{
    m_sections = sections;
}


void RootMenu::SetNumColumns(int numColumns)
{
    m_numCols = numColumns;
}


void RootMenu::Update()
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

    m_numRows = ceil(m_numItems / (double)m_numCols);
    int numFullRows = floor(m_numItems / (double)m_numCols);

    if (m_pColWidths != NULL) delete[] m_pColWidths;
    m_pColWidths = new int[m_numCols];

    for (int i=0; i < m_numCols; i++) m_pColWidths[i] = 0;


    m_singleCellHeight = m_fontMetrics.height() + (m_yPadding * 2);
    int totalHeight = m_numRows * m_singleCellHeight;

    QRectF textBounds;

    int itemIndex = 0;
    int currentRow = 0;
    int currentCol = 0;
    int lastRowWidth = 0;
    int standardRowWidth = 0;

    while (currentRow < numFullRows)
    {
        textBounds = m_fontMetrics.boundingRect(m_items.at(itemIndex));
        if (textBounds.width() > m_pColWidths[currentCol])
        {
            m_pColWidths[currentCol] = textBounds.width();
        }

        if (currentRow == (numFullRows-1))
        {
            m_pColWidths[currentCol] += (m_xPadding * 2);
            standardRowWidth += m_pColWidths[currentCol];
        }

        currentCol++;

        if (currentCol == m_numCols)
        {
            currentCol = 0;
            currentRow++;
        }

        itemIndex++;
    }

    while (itemIndex < m_numItems)
    {
        textBounds = m_fontMetrics.boundingRect(m_items.at(itemIndex));
        lastRowWidth += textBounds.width() + (m_xPadding * 2);
        itemIndex++;
    }

    int totalWidth = (standardRowWidth > lastRowWidth) ?
                      standardRowWidth : lastRowWidth;

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

    m_lastRowLeft = 0;
    m_standardRowLeft = 0;

    if (standardRowWidth > lastRowWidth)
    {
        m_lastRowLeft = (standardRowWidth - lastRowWidth) / 2;
    }
    else
    {
        m_standardRowLeft = (lastRowWidth - standardRowWidth) / 2;
    }

    QRectF boundingRect(m_standardRowLeft, 0, 1, m_singleCellHeight);


    while (currentRow < numFullRows)
    {
        boundingRect.setWidth(m_pColWidths[currentCol]);

        if (itemIndex == m_sections[sectionIndex])
        {
            sectionIndex++;
            fillIndex = sectionIndex % BOI_ROOT_MENU_NUM_FILLS;
        }

        painter.setPen(m_borderPen);
        painter.setBrush(m_fills[fillIndex]);
        painter.drawRect(boundingRect);

        painter.setPen(m_textPen);
        painter.drawText(boundingRect, Qt::AlignCenter, m_items.at(itemIndex));

        boundingRect.translate(m_pColWidths[currentCol], 0);

        currentCol++;

        if (currentCol == m_numCols)
        {
            currentCol = 0;
            currentRow++;

            boundingRect.translate(0, m_singleCellHeight);
            boundingRect.moveLeft(m_standardRowLeft);
        }

        itemIndex++;
    }

    boundingRect.moveLeft(m_lastRowLeft);

    while (itemIndex < m_numItems)
    {
        QString text = m_items.at(itemIndex);

        textBounds = m_fontMetrics.boundingRect(text);
        boundingRect.setWidth(textBounds.width() + (m_xPadding * 2));

        if (itemIndex == m_sections[sectionIndex])
        {
            sectionIndex++;
            fillIndex = sectionIndex % BOI_ROOT_MENU_NUM_FILLS;
        }

        painter.setPen(m_borderPen);
        painter.setBrush(m_fills[fillIndex]);
        painter.drawRect(boundingRect);

        painter.setPen(m_textPen);
        painter.drawText(boundingRect, Qt::AlignCenter, text);

        boundingRect.translate(textBounds.width() + (m_xPadding * 2), 0);

        itemIndex++;
    }
}


const QRectF& RootMenu::BoundingRect() const
{
    return m_boundingRect;
}


void RootMenu::Draw(QPainter* painter, const QRectF& rect)
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


int RootMenu::IndexAt(const QPointF& point)
{
    if (m_numItems > 0)
    {
        if (m_boundingRect.isValid() &&
            m_boundingRect.contains(point))
        {
            if (m_numItems == 1) return 0;

            qreal x = point.x();
            qreal y = point.y();

            int row = floor(y / m_singleCellHeight);
            int numExtraCells = m_numItems % m_numCols;

            if ((row == (m_numRows-1)) && (numExtraCells != 0))
            {
                if (x < m_lastRowLeft) return -1;

                /*
                 * TODO: speed this up by saving the widths when
                 * the menu is updated so that the bounding rect
                 * does not need to be retrieved here?
                 */

                QRectF textBounds;
                int currentWidth = m_lastRowLeft;
                int itemIndex = m_numItems - numExtraCells;

                while (itemIndex < m_numItems)
                {
                    textBounds = m_fontMetrics.boundingRect(m_items.at(itemIndex));
                    currentWidth += textBounds.width() + (m_xPadding * 2);

                    if (x < currentWidth)
                    {
                        return itemIndex;
                    }

                    itemIndex++;
                }
            }
            else
            {
                if (x < m_standardRowLeft) return -1;

                int currentWidth = m_standardRowLeft;

                for (int i=0; i < m_numCols; i++)
                {
                    currentWidth += m_pColWidths[i];

                    if (x < currentWidth)
                    {
                        int index = (m_numCols * row) + i;

                        if (index < m_numItems)
                        {
                            return index;
                        }
                    }
                }
            }
        }
    }

    return -1;
}


} // namespace BOI

