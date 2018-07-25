#-------------------------------------------------
#
# Project created by QtCreator 2015-11-30T21:34:06
#
#-------------------------------------------------

contains(CONFIG,release) {
  TARGET = intellect
} else {
  TARGET = intellectd
}

TEMPLATE = app

include(../common.pri)

CONFIG += mobility
CONFIG += precompile_header
QT += core gui
//QT += script scripttools
QT += qml quick positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#MOBILITY =


win32 {
  DESTDIR = $${DEST_BINS}
}

message("DESTDIR="$$DESTDIR)
#message($$target.files)

android {
#  DESTDIR = ../apk
  OTHER_FILES += ../platform-specific/android/AndroidManifest.xml
  ANDROID_PACKAGE_SOURCE_DIR = $$PWD/../platform-specific/android
#  UI_DIR = .ui
#  MOC_DIR = .moc
#  OBJECTS_DIR = .obj
}

include(intellect.pri)

