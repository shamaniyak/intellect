#-------------------------------------------------
#
# Project created by QtCreator 2017-05-27T01:31:04
#
#-------------------------------------------------

QT  += core
QT       -= gui

TARGET = SppzSituationToQt
TEMPLATE = lib
CONFIG += c++14 plugin

win32 {
  DESTDIR = ../../exe/plugins
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

DEFINES += SPPZSITUATIONTOQT_LIBRARY

INCLUDEPATH += \
  "../.." \
  "d:/Work/sppz/Include/Classifier" \
  "d:/Work/sppz/Include/situation"

SOURCES += sppzsituationtoqt.cpp

HEADERS += sppzsituationtoqt.h\
        sppzsituationtoqt_global.h \
    ../../Interfaces/isppzsituationplugin.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
