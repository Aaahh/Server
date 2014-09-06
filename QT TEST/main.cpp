#include "qttest.h"
#include <QtWidgets/QApplication>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <conio.h>
#include "Header.h"

int ServerMain(LPVOID pointerToObject);
int yes = 0;

DWORD WINAPI ChatFunc(LPVOID randparam)
{
    ServerMain(randparam);
    return 0;
}

HANDLE hChatThread;
DWORD dwChatThreadId;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTTEST w;
    w.show();
    hChatThread = CreateThread(NULL,0,ChatFunc,&w,0,&dwChatThreadId);
    return a.exec();
}

#undef UNICODE

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

int minitialize();
char *vir = "true";

DWORD WINAPI recvfunc(LPVOID pointerToObject)
{
    while (true) {
		char* bal = recv_client(pointerToObject);
		std::cout<<"BAL: "<<bal<<"\n";
        if (strncmp( bal, "1", 1) == 0) {
			std::cout<<"RETURNED PROPERLY!\n";
            return 1;
        }
    }
    return 0;
}

int ServerMain(LPVOID pointerToObject)
{
    minitialize();
    HANDLE hRecvThread;
    DWORD dwRecvThreadId;

    AllocConsole() ;
    AttachConsole( GetCurrentProcessId() ) ;
    freopen( "CON", "w", stdout ) ;

    // Accept a client socket
    for (int sock=1; sock<1000; sock++) {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        char sockprint[80];
        char sockchar[4];
        itoa(sock,sockchar,10);
        strcpy(sockprint,"Client ");
        strcat(sockprint,sockchar);
        strcat(sockprint," connected.");
        printm(sockprint);
        GoToXY(0,23);
        if (ClientSocket == INVALID_SOCKET) {
            std::cout<<"accept failed with error: "<<WSAGetLastError()<<"\n";
            closesocket(ClientSocket);
            WSACleanup();
            minitialize();
        }
        client[sock].cs=ClientSocket;
        client[sock].con=true;
        client[sock].i=clientnumber;
        client[sock].client=sock;
        lastclient=clientnumber;
			hRecvThread = CreateThread(NULL,0,recvfunc,pointerToObject,0,&dwRecvThreadId);
        clientnumber++;
        currentclient=clientnumber;
    }

    while(true)
    {
        Sleep(1000000);
    }

    // shutdown the connection since we're done
    mshutdown();

    return 0;
}



int minitialize()	//initialize the winsock server
{
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printm("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printm("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printm("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printm("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printm("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    unsigned long b=1;
    ioctlsocket(ClientSocket,FIONBIO,&b);


}


/*--------------------TEST FUNCTIONS--------------------*/