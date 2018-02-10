#-------------------------------------------------
#
# Project created by QtCreator 2017-11-24T21:30:44
#
#-------------------------------------------------

QT       += widgets

TARGET = WindowManager
TEMPLATE = lib
CONFIG += qt warn_on plugin c++14

DEFINES += WINDOWMANAGER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
  ../..

SOURCES += \
        windowmanager.cpp \
    ../../Src/GUI/MainWindowEx/mainwindowex.cpp \
    ../../Src/GUI/filedialog.cpp \
    ../../Src/GUI/graphicsview.cpp \
    ../../Src/GUI/logview.cpp \
    ../../Src/GUI/menu.cpp \
    ../../Src/GUI/statusbar.cpp \
    ../../Src/GUI/toolbar.cpp

HEADERS += \
        windowmanager.h \
        windowmanager_global.h \
    ../../Src/GUI/MainWindowEx/mainwindowex.h \
    ../../Src/GUI/filedialog.h \
    ../../Src/GUI/graphicsview.h \
    ../../Src/GUI/logview.h \
    ../../Src/GUI/menu.h \
    ../../Src/GUI/statusbar.h \
    ../../Src/GUI/toolbar.h

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

FORMS += \
    ../../Src/GUI/MainWindowEx/mainwindowex.ui
