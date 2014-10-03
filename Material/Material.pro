#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T09:44:38
#
#-------------------------------------------------

QT       += core gui widgets

#para compilar el estilo descomentar el DEFINES para la identidad correspondiente (por defecto xilema)
#DEFINES += xedro
#DEFINES += xabal
#DEFINES += xauce
#DEFINES += xavia
#DEFINES += xilema

TARGET = Material
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/styles

SOURCES += material_style_plugin.cpp \
    material_style.cpp \
    style_animation.cpp \
    progress_style_animation.cpp

HEADERS += material_style_plugin.h \
    material_style.h \
    constants.h \
    style_animation.h \
    progress_style_animation.h \
    constants_xilema.h \
    constants_xabal.h \
    constants_xauce.h \
    constants_xedro.h \
    constants_xavia.h
OTHER_FILES += Material.json \
    status.txt

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    resources.qrc
