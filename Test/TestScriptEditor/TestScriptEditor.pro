#-------------------------------------------------
#
# Project created by QtCreator 2018-04-20T23:25:26
#
#-------------------------------------------------

QT       += core gui quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestScriptEditor
TEMPLATE = app

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
    ../../05

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../../05/Src/GUI/ScriptEditor/highlighter.cpp \
    ../../05/Src/GUI/ScriptEditor/scripteditor.cpp \
    ../../05/Third/qmlcreator/cpp/QMLHighlighter.cpp \
    ../../05/Third/qmlcreator/cpp/SyntaxHighlighter.cpp

HEADERS += \
        mainwindow.h \
    ../../05/Src/GUI/ScriptEditor/highlighter.h \
    ../../05/Src/GUI/ScriptEditor/scripteditor.h \
    ../../05/Third/qmlcreator/cpp/QMLHighlighter.h \
    ../../05/Third/qmlcreator/cpp/SyntaxHighlighter.h

FORMS += \
        mainwindow.ui
