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

#ifndef TEXIMAGE3D_P_H
#define TEXIMAGE3D_P_H

#include "context3d_p.h"
#include "abstractobject3d_p.h"

#include <QtCore/QUrl>
#include <QtGui/QImage>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class CanvasTextureImageFactory : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CanvasTextureImageFactory)

public:
    static QObject *texture_image_factory_provider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static CanvasTextureImageFactory *factory(QQmlEngine *engine);
    explicit CanvasTextureImageFactory(QQmlEngine *engine, QObject *parent = 0);
    ~CanvasTextureImageFactory();

    void handleImageLoadingStarted(CanvasTextureImage *image);
    void notifyLoadedImages();

    Q_INVOKABLE CanvasTextureImage* newTexImage();
private:
    QQmlEngine *m_qmlEngine;
    QList<CanvasTextureImage *> m_loadingImagesList;
};

class CanvasTextureImage : public CanvasAbstractObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CanvasTextureImage)
    Q_PROPERTY(QUrl src READ src WRITE setSrc NOTIFY srcChanged)
    Q_PROPERTY(TextureImageState imageState READ imageState NOTIFY imageStateChanged)
    Q_PROPERTY(int width READ width NOTIFY widthChanged)
    Q_PROPERTY(int height READ height NOTIFY heightChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_ENUMS(TextureImageState)

public:
    enum TextureImageState {
        INITIALIZED = 0,
        LOAD_PENDING,
        LOADING,
        LOADING_FINISHED,
        LOADING_ERROR
    };

    Q_INVOKABLE explicit CanvasTextureImage(CanvasTextureImageFactory *parent = 0);
    virtual ~CanvasTextureImage();

    Q_INVOKABLE CanvasTextureImage *create();
    Q_INVOKABLE ulong id();
    Q_INVOKABLE CanvasTextureImage *resize(int width, int height);

    QVariant *anything() const;
    void setAnything(QVariant *value);

    const QUrl &src() const;
    void setSrc(const QUrl &src);
    TextureImageState imageState() const;
    int width() const;
    int height() const;
    QString errorString() const;

    void emitImageLoaded();
    void emitImageLoadingError();

    void load();
    void handleReply(QNetworkReply *reply);
    QImage &getImage();
    uchar *convertToFormat(CanvasContext::glEnums format, bool flipY = false);

    friend QDebug operator<< (QDebug d, const CanvasTextureImage *buffer);

private:
    void setImageState(TextureImageState state);
    explicit CanvasTextureImage(const QImage &source, int width, int height, QObject *parent = 0);

signals:
    void srcChanged(QUrl source);
    void imageStateChanged(TextureImageState state);
    void widthChanged(int width);
    void heightChanged(int height);
    void errorStringChanged(const QString errorString);
    void anythingChanged(QVariant *value);
    void imageLoadingStarted(CanvasTextureImage *image);
    void imageLoaded(CanvasTextureImage *image);
    void imageLoadingFailed(CanvasTextureImage *image);

private:
    QNetworkAccessManager *m_networkAccessManager;
    QImage m_image;
    QUrl m_source;
    TextureImageState m_state;
    QString m_errorString;
    uchar *m_pixelCache;
    CanvasContext::glEnums m_pixelCacheFormat;
    bool m_pixelCacheFlipY;
    QImage m_glImage;
    QVariant *m_anyValue;
    CanvasTextureImageFactory *m_parentFactory;
};

#endif // TEXIMAGE3D_P_H
