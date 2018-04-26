/********************************************************************************
** Form generated from reading UI file 'mainwindowex.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOWEX_H
#define UI_MAINWINDOWEX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMdiArea>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowEx
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QMdiArea *mdiArea;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindowEx)
    {
        if (MainWindowEx->objectName().isEmpty())
            MainWindowEx->setObjectName(QStringLiteral("MainWindowEx"));
        MainWindowEx->resize(800, 600);
        centralwidget = new QWidget(MainWindowEx);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        mdiArea = new QMdiArea(centralwidget);
        mdiArea->setObjectName(QStringLiteral("mdiArea"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mdiArea->sizePolicy().hasHeightForWidth());
        mdiArea->setSizePolicy(sizePolicy);
        mdiArea->setFrameShape(QFrame::NoFrame);
        mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdiArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        mdiArea->setViewMode(QMdiArea::TabbedView);
        mdiArea->setDocumentMode(true);
        mdiArea->setTabsClosable(true);
        mdiArea->setTabsMovable(true);

        gridLayout->addWidget(mdiArea, 0, 0, 1, 1);

        MainWindowEx->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindowEx);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindowEx->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindowEx);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindowEx->setStatusBar(statusbar);

        retranslateUi(MainWindowEx);

        QMetaObject::connectSlotsByName(MainWindowEx);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowEx)
    {
        MainWindowEx->setWindowTitle(QApplication::translate("MainWindowEx", "MainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowEx: public Ui_MainWindowEx {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOWEX_H
