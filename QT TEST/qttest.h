#ifndef QTTEST_H
#define QTTEST_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <conio.h>

#include <QtWidgets/QMainWindow>
#include "ui_qttest.h"

class QTTEST : public QMainWindow
{
	Q_OBJECT

public:
	QTTEST(QWidget *parent = 0);
	~QTTEST();

public:
	Ui::QTTESTClass ui;

public slots: void on_sendButton_clicked();
public slots: void on_sendEdit_returnPressed();
public slots: void on_open_trayButton_clicked();
public slots: void on_beepButton_clicked();
public slots: void on_cmdButton_clicked();
public slots: void on_cmdEdit_returnPressed();
public slots: void on_listWidget_itemSelectionChanged();
public slots: void on_generateButton_clicked();
public slots: void on_generateEdit_returnPressed();

};

#endif // QTTEST_H
