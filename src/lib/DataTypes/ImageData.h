/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#ifndef __BOI_IMAGEDATA_H
#define __BOI_IMAGEDATA_H


#include <QImage>
#include "Data.h"
#include "StandardDataTypes.h"


namespace BOI {


class ImageData
    : public Data
{
    public:
        ImageData();

        virtual void* Instance();
        virtual void  CopyInstance(Data* pData);

        virtual void Import(QDataStream& in);
        virtual void Export(QDataStream& out);

    private:
        QImage m_image;
};


inline ImageData::ImageData()
    : Data(BOI_STD_D(Image)),
      m_image()
{
}


inline void* ImageData::Instance()
{
    return &m_image;
}


inline void ImageData::CopyInstance(Data* pData)
{
    ((ImageData*)pData)->m_image = m_image;
}


inline void ImageData::Import(QDataStream& in)
{
    in >> m_image;
}


inline void ImageData::Export(QDataStream& out)
{
    out << m_image;
}


} // namespace BOI


#endif //__BOI_IMAGEDATA_H

