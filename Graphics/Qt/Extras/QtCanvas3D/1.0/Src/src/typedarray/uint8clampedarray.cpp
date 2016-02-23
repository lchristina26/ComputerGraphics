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

#include "uint8clampedarray_p.h"
#include "../arrayutils_p.h"
#include "arraybuffer_p.h"

/*!
 * \qmltype Uint8ClampedArray
 * \since QtCanvas3D 1.0
 * \ingroup qtcanvas3d-qml-types
 * \brief Contains a typed array of clamped 8 bit unsigned ints.
 *
 * An uncreatable QML type that contains a typed array of clamped 8 bit unsigned ints.
 * Use \l Arrays::newUint8ClampedArray() for getting an array of this type.
 */

/*!
 * \fn virtual inline int CanvasUint8ClampedArray::bytesPerElement();
 * \internal
 */

/*!
 * \fn virtual inline CanvasContext3D::glEnums CanvasUint8ClampedArray::dataType();
 * \internal
 */

/*!
 * \internal
 */
CanvasUint8ClampedArray::CanvasUint8ClampedArray(QObject *parent) :
    CanvasTypedArray(0, 0, parent),
    m_typedBuffer(0)
{
    setLength(0);
    //qDebug() << "Uint8ClampedArray(QObject *parent)";
}

/*!
 * \internal
 */
CanvasUint8ClampedArray::CanvasUint8ClampedArray(unsigned long length, QObject *parent) :
    CanvasTypedArray(0, 0, parent),
    m_typedBuffer(0)
{
    setLength(length);
    //qDebug() << "Uint8ClampedArray(unsigned long length, QObject *parent)";
    if (length > 0) {
        m_buffer = new CanvasArrayBuffer(sizeof(unsigned char) * CanvasTypedArray::length());
        m_typedBuffer = (unsigned char *)m_buffer->m_rawData;
    }
}

/*!
 * \internal
 */
CanvasUint8ClampedArray::CanvasUint8ClampedArray(CanvasTypedArray *array, QObject *parent) :
    CanvasTypedArray(0, 0, parent)
{
    setLength(array->length());
    m_buffer = new CanvasArrayBuffer(CanvasTypedArray::length() * this->bytesPerElement());
    m_typedBuffer = (unsigned char *)m_buffer->m_rawData;

    if (!ArrayUtils::copyToTargetClampedByte(m_typedBuffer, array, CanvasTypedArray::length())) {
        // Conversion failed, make this an empty buffer
        delete m_buffer;
        setLength(0);
    }
}

/*!
 * \internal
 */
CanvasUint8ClampedArray::CanvasUint8ClampedArray(QVariantList *array, QObject *parent) :
    CanvasTypedArray(0, 0, parent)
{
    //qDebug() << "Uint8ClampedArray(const QVariantList *array[" << array->count() << "], QObject *parent)";
    setLength(array->count());
    m_buffer = new CanvasArrayBuffer(CanvasTypedArray::length() * sizeof(unsigned char), this);
    m_typedBuffer = (unsigned char *)m_buffer->m_rawData;
    int idx = 0;
    for (QVariantList::const_iterator it = array->begin(); it != array->end(); ++it) {
        QVariant element = *it;
        if (element.canConvert<unsigned char>()) {
            m_typedBuffer[idx] = (unsigned char)(element.toInt());
        } else {
            //qDebug() << "Failed conversion to unsigned byte of "<<element;
            m_typedBuffer[idx] = (unsigned char)0;
        }
        idx++;
    }
}

/*!
 * \internal
 */
CanvasUint8ClampedArray::CanvasUint8ClampedArray(CanvasArrayBuffer *buffer,
                                                 unsigned long byteOffset, QObject *parent) :
    CanvasTypedArray(buffer, byteOffset, parent)
{
    //qDebug() << "Uint8ClampedArray(ArrayBuffer *buffer, unsigned long byteOffset, QObject *parent)";
    m_typedBuffer = (unsigned char *)(m_buffer->m_rawData + byteOffset);
    setLength((byteLength() - byteOffset) / bytesPerElement());
}

/*!
 * \qmlmethod int Uint8ClampedArray::operator [] (int index)
 * Returns the array value at \a index.
 */
/*!
 * \internal
 */
unsigned char CanvasUint8ClampedArray::operator [] (unsigned long index)
{
    if (index < length())
        return m_typedBuffer[index];
    return uchar(0);
}

/*!
 * \qmlmethod int Uint8ClampedArray::get(int index)
 * Returns the array value at \a index.
 */
/*!
 * \internal
 */
unsigned char CanvasUint8ClampedArray::get(unsigned long index)
{
    return m_typedBuffer[index];
}

/*!
 * \qmlmethod void Uint8ClampedArray::set(int index, int value)
 * Sets the \a value to \a index in the array.
 */
/*!
 * \internal
 */
void CanvasUint8ClampedArray::set(unsigned long index, unsigned char value)
{
    m_typedBuffer[index] = value;
}

/*
void Uint8ClampedArray::set(TypedArray *array, unsigned long offset)
{

}

void Uint8ClampedArray::set(float *array, unsigned long offset)
{

}

TypedArray *Uint8ClampedArray::subarray(long begin)
{

}

TypedArray *Uint8ClampedArray::subarray(long begin, long end)
{

}
*/
