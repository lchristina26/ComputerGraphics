HEADERS += glwidget.h 
SOURCES += glwidget.cpp main.cpp

QT += opengl
CONFIG -= app_bundle
CONFIG += console
INCLUDEPATH += "../include"
LIBS += -L/usr/local/lib -lfreeimage

RESOURCES += \
    shaders.qrc \
    image.qrc

DISTFILES += \
    moonlight_night-wallpaper-1024x576.jpg \
    moonlight.jpg
