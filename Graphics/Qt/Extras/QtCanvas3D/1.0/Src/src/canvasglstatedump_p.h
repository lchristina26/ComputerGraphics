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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtCanvas3D API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef CANVASGLSTATEDUMP_P_H
#define CANVASGLSTATEDUMP_P_H

#include "canvas3dcommon_p.h"

#include <QObject>
#include <QtGui/QOpenGLFunctions>

#define DUMP_ENUM_AS_PROPERTY(A,B,C) Q_PROPERTY(A::B C READ C ## _read); inline A::B C ## _read() { return A::C; }

class EnumToStringMap;

class CanvasGLStateDump : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT

    Q_ENUMS(stateDumpEnums)

public:
    enum stateDumpEnums {
        DUMP_BASIC_ONLY                        = 0x00,
        DUMP_VERTEX_ATTRIB_ARRAYS_BIT          = 0x01,
        DUMP_VERTEX_ATTRIB_ARRAYS_BUFFERS_BIT  = 0x02,
        DUMP_FULL                              = 0x03
    };

    DUMP_ENUM_AS_PROPERTY(CanvasGLStateDump,stateDumpEnums,DUMP_BASIC_ONLY)
    DUMP_ENUM_AS_PROPERTY(CanvasGLStateDump,stateDumpEnums,DUMP_VERTEX_ATTRIB_ARRAYS_BIT)
    DUMP_ENUM_AS_PROPERTY(CanvasGLStateDump,stateDumpEnums,DUMP_VERTEX_ATTRIB_ARRAYS_BUFFERS_BIT)
    DUMP_ENUM_AS_PROPERTY(CanvasGLStateDump,stateDumpEnums,DUMP_FULL)

    CanvasGLStateDump(QOpenGLContext *context, QObject *parent = 0);
    ~CanvasGLStateDump();

    Q_INVOKABLE QString getGLStateDump(stateDumpEnums options = DUMP_BASIC_ONLY);

    QString getGLArrayObjectDump(int target, int arrayObject, int type);

private:
    GLint m_maxVertexAttribs;
    EnumToStringMap *m_map;
    bool m_isOpenGLES2;
};

#endif // CANVASGLSTATEDUMP_P_H
