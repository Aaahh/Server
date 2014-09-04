#include "qttest.h"
#include <QtWidgets/QApplication>
#ifndef __Header_H_DEFINED__
#define __Header_H_DEFINED__
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <string>

int msend(char msendbuf[512]);
int msend(LPVOID pointerToObject, char msendbuf[512]);
char* mrecv(bool show);
int change_client(int client);
int mexit();

extern int currentclient;
extern SOCKET ClientSocket;

QTTEST::QTTEST(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void QTTEST::on_sendButton_clicked() {

	std::string current_locale_text = (ui.sendEdit->text()).toLocal8Bit().constData();
	char *cstr = new char[current_locale_text.length() + 1];
	strcpy(cstr, current_locale_text.c_str());
	char sends[512]="Server> ";
    strcat(sends,cstr);
    msend(this, sends);
    ui.textBrowser->append(sends); //Changes the text on the Text-label
	ui.sendEdit->setText("");

}

void QTTEST::on_sendEdit_returnPressed() {

	std::string current_locale_text = (ui.sendEdit->text()).toLocal8Bit().constData();
	char *cstr = new char[current_locale_text.length() + 1];
	strcpy(cstr, current_locale_text.c_str());
	char sends[512]="Server> ";
    strcat(sends,cstr);
    msend(this, sends);
    ui.textBrowser->append(sends); //Changes the text on the Text-label
	ui.sendEdit->setText("");

}

void QTTEST::on_open_trayButton_clicked() {

	msend("Server> /open_tray");

}

void QTTEST::on_cmdEdit_returnPressed() {

	char sends[512]="Server> /cmd ";
	std::string cmd_text = (ui.cmdEdit->text()).toLocal8Bit().constData();
	char *cstr = new char[cmd_text.length() + 1];
	strcpy(cstr, cmd_text.c_str());
	strcat(sends, cstr);
	msend(sends);
	ui.cmdEdit->setText("");
}

void QTTEST::on_cmdButton_clicked() {

	char sends[512]="Server> /cmd ";
	std::string cmd_text = (ui.cmdEdit->text()).toLocal8Bit().constData();
	char *cstr = new char[cmd_text.length() + 1];
	strcpy(cstr, cmd_text.c_str());
	strcat(sends, cstr);
	msend(sends);
}

void QTTEST::on_beepButton_clicked() {

	char sends[512]="Server> /beep ";
	std::string pitch_text = (ui.pitchEdit->text()).toLocal8Bit().constData();
	char *cstr = new char[pitch_text.length() + 1];
	strcpy(cstr, pitch_text.c_str());
	std::string length_text = (ui.lengthEdit->text()).toLocal8Bit().constData();
	char *cstr2 = new char[length_text.length() + 1];
	strcpy(cstr2, length_text.c_str());
	strcat(sends, cstr);
	strcat(sends, " ");
	strcat(sends, cstr2);
	msend(sends);
}

void QTTEST::on_listWidget_itemSelectionChanged() {

	int nm = (ui.listWidget->currentIndex().row()) + 1;
	change_client(nm);

}

QTTEST::~QTTEST()
{

}