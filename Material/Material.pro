#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T09:44:38
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Material
TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$[QT_INSTALL_PLUGINS]/styles

SOURCES += material_style_plugin.cpp \
    material_style.cpp

HEADERS += material_style_plugin.h \
    material_style.h \
    constants.h
OTHER_FILES += Material.json \
    status.txt

unix {
    target.path = /usr/lib
    INSTALLS += target
}
