/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCanvas3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free 
** Software Foundation and appearing in the file LICENSE.GPL included in 
** the packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "renderbuffer3d_p.h"

/*!
 * \qmltype RenderBuffer3D
 * \since QtCanvas3D 1.0
 * \ingroup qtcanvas3d-qml-types
 * \brief Contains an OpenGL renderbuffer.
 *
 * An uncreatable QML type that contains an OpenGL renderbuffer. You can get it by calling
 * \l{Context3D::createRenderbuffer()}{Context3D.createRenderbuffer()} method.
 */

/*!
 * \internal
 */
CanvasRenderBuffer::CanvasRenderBuffer(QObject *parent) :
    CanvasAbstractObject(parent)
{
    initializeOpenGLFunctions();
    glGenRenderbuffers(1, &m_renderbufferId);
}


/*!
 * \internal
 */
CanvasRenderBuffer::~CanvasRenderBuffer()
{
    del();
}

/*!
 * \internal
 */
bool CanvasRenderBuffer::isAlive()
{
    return (m_renderbufferId);
}

/*!
 * \internal
 */
void CanvasRenderBuffer::del()
{
    if (m_renderbufferId) {
        glDeleteRenderbuffers(1, &m_renderbufferId);
        m_renderbufferId = 0;
    }
}

/*!
 * \internal
 */
GLuint CanvasRenderBuffer::id()
{
    return m_renderbufferId;
}