/********************************************************************************
** Form generated from reading UI file 'qttest.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTTEST_H
#define UI_QTTEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QTTESTClass
{
public:
    QAction *actionCommand_List;
    QWidget *centralWidget;
    QLineEdit *pitchEdit;
    QTextBrowser *textBrowser;
    QPushButton *sendButton;
    QLineEdit *sendEdit;
    QPushButton *beepButton;
    QLineEdit *lengthEdit;
    QPushButton *open_trayButton;
    QPushButton *cmdButton;
    QLineEdit *cmdEdit;
    QListWidget *listWidget;
    QPushButton *generateButton;
    QLineEdit *generateEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QTTESTClass)
    {
        if (QTTESTClass->objectName().isEmpty())
            QTTESTClass->setObjectName(QStringLiteral("QTTESTClass"));
        QTTESTClass->resize(796, 402);
        actionCommand_List = new QAction(QTTESTClass);
        actionCommand_List->setObjectName(QStringLiteral("actionCommand_List"));
        centralWidget = new QWidget(QTTESTClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pitchEdit = new QLineEdit(centralWidget);
        pitchEdit->setObjectName(QStringLiteral("pitchEdit"));
        pitchEdit->setGeometry(QRect(120, 20, 31, 20));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setEnabled(true);
        textBrowser->setGeometry(QRect(360, 0, 431, 321));
        sendButton = new QPushButton(centralWidget);
        sendButton->setObjectName(QStringLiteral("sendButton"));
        sendButton->setGeometry(QRect(710, 320, 75, 23));
        sendEdit = new QLineEdit(centralWidget);
        sendEdit->setObjectName(QStringLiteral("sendEdit"));
        sendEdit->setGeometry(QRect(360, 320, 351, 20));
        beepButton = new QPushButton(centralWidget);
        beepButton->setObjectName(QStringLiteral("beepButton"));
        beepButton->setGeometry(QRect(0, 20, 121, 23));
        lengthEdit = new QLineEdit(centralWidget);
        lengthEdit->setObjectName(QStringLiteral("lengthEdit"));
        lengthEdit->setGeometry(QRect(150, 20, 31, 20));
        open_trayButton = new QPushButton(centralWidget);
        open_trayButton->setObjectName(QStringLiteral("open_trayButton"));
        open_trayButton->setGeometry(QRect(0, 40, 75, 23));
        cmdButton = new QPushButton(centralWidget);
        cmdButton->setObjectName(QStringLiteral("cmdButton"));
        cmdButton->setGeometry(QRect(0, 60, 75, 23));
        cmdEdit = new QLineEdit(centralWidget);
        cmdEdit->setObjectName(QStringLiteral("cmdEdit"));
        cmdEdit->setGeometry(QRect(80, 60, 281, 21));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(0, 80, 361, 241));
        generateButton = new QPushButton(centralWidget);
        generateButton->setObjectName(QStringLiteral("generateButton"));
        generateButton->setGeometry(QRect(0, 0, 101, 23));
        generateEdit = new QLineEdit(centralWidget);
        generateEdit->setObjectName(QStringLiteral("generateEdit"));
        generateEdit->setGeometry(QRect(100, 0, 161, 20));
        QTTESTClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QTTESTClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 796, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        QTTESTClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QTTESTClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QTTESTClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QTTESTClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QTTESTClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionCommand_List);

        retranslateUi(QTTESTClass);

        QMetaObject::connectSlotsByName(QTTESTClass);
    } // setupUi

    void retranslateUi(QMainWindow *QTTESTClass)
    {
        QTTESTClass->setWindowTitle(QApplication::translate("QTTESTClass", "QTTEST", 0));
        actionCommand_List->setText(QApplication::translate("QTTESTClass", "Command List", 0));
        sendButton->setText(QApplication::translate("QTTESTClass", "Send", 0));
        beepButton->setText(QApplication::translate("QTTESTClass", "Beep [pitch] [length]:", 0));
        open_trayButton->setText(QApplication::translate("QTTESTClass", "Open Tray", 0));
        cmdButton->setText(QApplication::translate("QTTESTClass", "CMD:", 0));
        generateButton->setText(QApplication::translate("QTTESTClass", "Generate Payload", 0));
        menuFile->setTitle(QApplication::translate("QTTESTClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class QTTESTClass: public Ui_QTTESTClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTTEST_H
