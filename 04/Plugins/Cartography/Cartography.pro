#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T16:17:10
#
#-------------------------------------------------

QT += winextras widgets

TARGET = Cartography
TEMPLATE = lib
CONFIG += qt warn_on plugin c++14

DEFINES += CARTOGRAPHY_LIBRARY _USE_MATH_DEFINES

INCLUDEPATH += \
  Src/MapView \
  Src/MapView/Include \
  ../../Interfaces \
  "d:/work/sppz/Include"

SOURCES += Src/sppzcartography.cpp \
    Src/MapView/MapView.cpp \
    Src/cartography.cpp \
    Src/cartographymap.cpp \
    Src/MapView/mapgraphicspixmapitem.cpp

HEADERS += Src/sppzcartography.h\
        Src/sppzcartography_global.h \
    Src/MapView/MapView.h \
    Src/cartography.h \
    Src/cartographymap.h \
    Src/MapView/mapgraphicspixmapitem.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    DESTDIR = ../../Exe/Plugins
    UI_DIR = .ui/win
    MOC_DIR = .moc/win
    OBJECTS_DIR = .obj/win
}
