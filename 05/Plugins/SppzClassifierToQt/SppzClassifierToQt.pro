#-------------------------------------------------
#
# Project created by QtCreator 2017-05-16T01:37:33
#
#-------------------------------------------------

QT  += core
QT       -= gui

TARGET = SppzClassifierToQt
TEMPLATE = lib
CONFIG += c++14 plugin

win32 {
  DESTDIR = ../../exe/plugins
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

DEFINES += SPPZCLASSIFIERTOQT_LIBRARY

INCLUDEPATH += \
  "../.." \
  "../../Interfaces/classifier" \
  "d:/Work/sppz/Include/Classifier"

SOURCES += sppzclassifiertoqt.cpp

HEADERS += sppzclassifiertoqt.h\
        sppzclassifiertoqt_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
