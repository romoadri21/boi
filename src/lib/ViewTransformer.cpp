/* Copyright (c) 2010, Piet Hein Schouten. All rights reserved.
 * This code is licensed under a BSD-style license that can be
 * found in the LICENSE file. The license can also be found at:
 * http://www.boi-project.org/license
 */

#include "View.h"
#include "ViewTransformer.h"


namespace BOI {


ViewTransformer::ViewTransformer()
    : m_pView(NULL),
      m_pNext(NULL)
{
}


bool ViewTransformer::Initialize(int version, qreal* pArgs)
{
    Q_UNUSED(version);
    Q_UNUSED(pArgs);

    return true;
}


bool ViewTransformer::Run()
{
    return false;
}


void ViewTransformer::Reset()
{
}


void ViewTransformer::Translate(qreal x, qreal y)
{
    m_pView->Translate(x, y);
}


void ViewTransformer::Scale(qreal scaleFactor)
{
    m_pView->Scale(scaleFactor);
}


} // namespace BOI

