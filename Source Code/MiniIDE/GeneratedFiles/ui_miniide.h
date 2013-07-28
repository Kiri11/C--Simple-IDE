/********************************************************************************
** Form generated from reading UI file 'miniide.ui'
**
** Created: Sun 28. Jul 12:36:45 2013
**      by: Qt User Interface Compiler version 5.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MINIIDE_H
#define UI_MINIIDE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MiniIDEClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QPlainTextEdit *mSourceCode;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QPlainTextEdit *mParseTree;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QPlainTextEdit *mSymbolTable;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QPlainTextEdit *mConsoleOutput;
    QHBoxLayout *horizontalLayout;
    QPushButton *mOpenButton;
    QPushButton *mSaveButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *mScanButton;
    QPushButton *mCompileButton;
    QPushButton *mCompileAndRunButton;

    void setupUi(QMainWindow *MiniIDEClass)
    {
        if (MiniIDEClass->objectName().isEmpty())
            MiniIDEClass->setObjectName(QStringLiteral("MiniIDEClass"));
        MiniIDEClass->resize(800, 500);
        centralWidget = new QWidget(MiniIDEClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_6 = new QVBoxLayout(centralWidget);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        mSourceCode = new QPlainTextEdit(centralWidget);
        mSourceCode->setObjectName(QStringLiteral("mSourceCode"));
        QFont font;
        font.setFamily(QStringLiteral("Courier New"));
        font.setPointSize(8);
        mSourceCode->setFont(font);

        verticalLayout_2->addWidget(mSourceCode);


        horizontalLayout_2->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3);

        mParseTree = new QPlainTextEdit(centralWidget);
        mParseTree->setObjectName(QStringLiteral("mParseTree"));
        mParseTree->setEnabled(true);
        QPalette palette;
        QBrush brush(QColor(240, 240, 240, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        mParseTree->setPalette(palette);
        QFont font1;
        font1.setFamily(QStringLiteral("Courier New"));
        mParseTree->setFont(font1);
        mParseTree->setAutoFillBackground(false);
        mParseTree->setReadOnly(true);

        verticalLayout_3->addWidget(mParseTree);


        horizontalLayout_2->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_4->addWidget(label_4);

        mSymbolTable = new QPlainTextEdit(centralWidget);
        mSymbolTable->setObjectName(QStringLiteral("mSymbolTable"));
        mSymbolTable->setEnabled(true);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        mSymbolTable->setPalette(palette1);
        mSymbolTable->setFont(font1);
        mSymbolTable->setReadOnly(true);

        verticalLayout_4->addWidget(mSymbolTable);


        horizontalLayout_2->addLayout(verticalLayout_4);

        horizontalLayout_2->setStretch(0, 4);
        horizontalLayout_2->setStretch(1, 3);
        horizontalLayout_2->setStretch(2, 2);

        verticalLayout_5->addLayout(horizontalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        mConsoleOutput = new QPlainTextEdit(centralWidget);
        mConsoleOutput->setObjectName(QStringLiteral("mConsoleOutput"));
        mConsoleOutput->setEnabled(true);
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush);
        mConsoleOutput->setPalette(palette2);
        mConsoleOutput->setFont(font1);
        mConsoleOutput->setReadOnly(true);

        verticalLayout->addWidget(mConsoleOutput);


        verticalLayout_5->addLayout(verticalLayout);

        verticalLayout_5->setStretch(0, 3);
        verticalLayout_5->setStretch(1, 2);

        verticalLayout_6->addLayout(verticalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        mOpenButton = new QPushButton(centralWidget);
        mOpenButton->setObjectName(QStringLiteral("mOpenButton"));

        horizontalLayout->addWidget(mOpenButton);

        mSaveButton = new QPushButton(centralWidget);
        mSaveButton->setObjectName(QStringLiteral("mSaveButton"));

        horizontalLayout->addWidget(mSaveButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        mScanButton = new QPushButton(centralWidget);
        mScanButton->setObjectName(QStringLiteral("mScanButton"));

        horizontalLayout->addWidget(mScanButton);

        mCompileButton = new QPushButton(centralWidget);
        mCompileButton->setObjectName(QStringLiteral("mCompileButton"));

        horizontalLayout->addWidget(mCompileButton);

        mCompileAndRunButton = new QPushButton(centralWidget);
        mCompileAndRunButton->setObjectName(QStringLiteral("mCompileAndRunButton"));

        horizontalLayout->addWidget(mCompileAndRunButton);


        verticalLayout_6->addLayout(horizontalLayout);

        MiniIDEClass->setCentralWidget(centralWidget);

        retranslateUi(MiniIDEClass);

        QMetaObject::connectSlotsByName(MiniIDEClass);
    } // setupUi

    void retranslateUi(QMainWindow *MiniIDEClass)
    {
        MiniIDEClass->setWindowTitle(QApplication::translate("MiniIDEClass", "MiniIDE", 0));
        label_2->setText(QApplication::translate("MiniIDEClass", "Source Code", 0));
        label_3->setText(QApplication::translate("MiniIDEClass", "Parse Tree", 0));
        label_4->setText(QApplication::translate("MiniIDEClass", "Symbol Table", 0));
        label->setText(QApplication::translate("MiniIDEClass", "Console Output", 0));
        mOpenButton->setText(QApplication::translate("MiniIDEClass", "Open...", 0));
        mSaveButton->setText(QApplication::translate("MiniIDEClass", "Save", 0));
        mScanButton->setText(QApplication::translate("MiniIDEClass", "Scan", 0));
        mCompileButton->setText(QApplication::translate("MiniIDEClass", "Compile", 0));
        mCompileAndRunButton->setText(QApplication::translate("MiniIDEClass", "Compile and Run", 0));
    } // retranslateUi

};

namespace Ui {
    class MiniIDEClass: public Ui_MiniIDEClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MINIIDE_H
