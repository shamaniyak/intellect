TEMPLATE = lib
TARGET = MemoryManager
QT += qml quick
CONFIG += plugin c++14

TARGET = $$qtLibraryTarget($$TARGET)
uri = memorymanagerplugins

# Input
SOURCES += \
    memorymanager_plugin.cpp \
    memorymanager.cpp

HEADERS += \
    memorymanager_plugin.h \
    memorymanager.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

win32: {
    MOC_DIR = .MOC
    OBJECTS_DIR = .OBJ
    UI_DIR = .UI
    DESTDIR = ../bin
}
