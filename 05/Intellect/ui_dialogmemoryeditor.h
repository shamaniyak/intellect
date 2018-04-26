/********************************************************************************
** Form generated from reading UI file 'dialogmemoryeditor.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGMEMORYEDITOR_H
#define UI_DIALOGMEMORYEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Editor
{
public:
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *edtName;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout;
    QTextEdit *edtPath;
    QSplitter *splitter;
    QGroupBox *grpVal;
    QVBoxLayout *verticalLayout_2;
    QPlainTextEdit *txtVal;
    QGroupBox *grpResult;
    QVBoxLayout *verticalLayout_3;
    QPlainTextEdit *txtResult;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAdd;
    QPushButton *btnEdit;
    QPushButton *btnDelete;
    QPushButton *btnCompile;

    void setupUi(QWidget *Editor)
    {
        if (Editor->objectName().isEmpty())
            Editor->setObjectName(QStringLiteral("Editor"));
        Editor->resize(712, 643);
        verticalLayout_5 = new QVBoxLayout(Editor);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        groupBox_5 = new QGroupBox(Editor);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setMinimumSize(QSize(0, 22));
        groupBox_5->setMaximumSize(QSize(16777215, 22));
        groupBox_5->setAutoFillBackground(true);
        groupBox_5->setFlat(false);

        verticalLayout_5->addWidget(groupBox_5);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(6, -1, -1, -1);
        label_2 = new QLabel(Editor);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        edtName = new QLineEdit(Editor);
        edtName->setObjectName(QStringLiteral("edtName"));

        horizontalLayout_2->addWidget(edtName);


        verticalLayout_4->addLayout(horizontalLayout_2);

        groupBox_3 = new QGroupBox(Editor);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setMaximumSize(QSize(16777215, 80));
        verticalLayout = new QVBoxLayout(groupBox_3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        edtPath = new QTextEdit(groupBox_3);
        edtPath->setObjectName(QStringLiteral("edtPath"));
        edtPath->setAutoFillBackground(false);
        edtPath->setReadOnly(true);

        verticalLayout->addWidget(edtPath);


        verticalLayout_4->addWidget(groupBox_3);

        splitter = new QSplitter(Editor);
        splitter->setObjectName(QStringLiteral("splitter"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
        splitter->setSizePolicy(sizePolicy);
        splitter->setMinimumSize(QSize(100, 0));
        splitter->setOrientation(Qt::Vertical);
        splitter->setHandleWidth(5);
        grpVal = new QGroupBox(splitter);
        grpVal->setObjectName(QStringLiteral("grpVal"));
        verticalLayout_2 = new QVBoxLayout(grpVal);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        txtVal = new QPlainTextEdit(grpVal);
        txtVal->setObjectName(QStringLiteral("txtVal"));
        txtVal->setLineWrapMode(QPlainTextEdit::NoWrap);

        verticalLayout_2->addWidget(txtVal);

        splitter->addWidget(grpVal);
        grpResult = new QGroupBox(splitter);
        grpResult->setObjectName(QStringLiteral("grpResult"));
        verticalLayout_3 = new QVBoxLayout(grpResult);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        txtResult = new QPlainTextEdit(grpResult);
        txtResult->setObjectName(QStringLiteral("txtResult"));

        verticalLayout_3->addWidget(txtResult);

        splitter->addWidget(grpResult);

        verticalLayout_4->addWidget(splitter);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btnAdd = new QPushButton(Editor);
        btnAdd->setObjectName(QStringLiteral("btnAdd"));

        horizontalLayout->addWidget(btnAdd);

        btnEdit = new QPushButton(Editor);
        btnEdit->setObjectName(QStringLiteral("btnEdit"));

        horizontalLayout->addWidget(btnEdit);

        btnDelete = new QPushButton(Editor);
        btnDelete->setObjectName(QStringLiteral("btnDelete"));

        horizontalLayout->addWidget(btnDelete);

        btnCompile = new QPushButton(Editor);
        btnCompile->setObjectName(QStringLiteral("btnCompile"));

        horizontalLayout->addWidget(btnCompile);


        verticalLayout_4->addLayout(horizontalLayout);


        verticalLayout_5->addLayout(verticalLayout_4);

        groupBox_5->raise();

        retranslateUi(Editor);

        QMetaObject::connectSlotsByName(Editor);
    } // setupUi

    void retranslateUi(QWidget *Editor)
    {
        Editor->setWindowTitle(QApplication::translate("Editor", "\320\221\320\260\320\267\320\260 \320\267\320\275\320\260\320\275\320\270\320\271", nullptr));
        groupBox_5->setTitle(QString());
        label_2->setText(QApplication::translate("Editor", "\320\230\320\274\321\217", nullptr));
        groupBox_3->setTitle(QApplication::translate("Editor", "\320\237\321\203\321\202\321\214", nullptr));
        edtPath->setHtml(QApplication::translate("Editor", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        grpVal->setTitle(QApplication::translate("Editor", "\320\227\320\275\320\260\321\207\320\265\320\275\320\270\320\265", nullptr));
        grpResult->setTitle(QApplication::translate("Editor", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202", nullptr));
        btnAdd->setText(QApplication::translate("Editor", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        btnEdit->setText(QApplication::translate("Editor", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        btnDelete->setText(QApplication::translate("Editor", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        btnCompile->setText(QApplication::translate("Editor", "\320\222\321\213\320\277\320\276\320\273\320\275\320\270\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Editor: public Ui_Editor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGMEMORYEDITOR_H
