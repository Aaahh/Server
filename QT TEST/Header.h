#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>
#include "qttest.h"

#pragma comment(lib, "advapi32.lib") //For token interaction

int clientnumber=0;
int currentclient=0;
int lastclient=0;

SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "10150"

WSADATA wsaData;
int iResult;

struct addrinfo *result = NULL;
struct addrinfo hints;

int iSendResult;
char recvbuf[DEFAULT_BUFLEN];
int recvbuflen = DEFAULT_BUFLEN;
int rows=0;
int verbose = 0;

int msend(char msendbuf[1024]);
int msend(LPVOID pointerToObject, char msendbuf[1024]);
int printm(char *inp);
int printm(char *inp, DWORD color);
int printm(char *inp, LPVOID pointerToObject);
int mshutdown();
void disconnect(LPVOID pointerToObject, int on);
void disconnect(int on);
void GoToXY(int column, int line);
int scroll(void);
int mexit();
int change_client(int client);
void mbroadcast(char* mess);
char* recv_client(LPVOID pointerToObject);
std::string enc(std::string data);
std::string readTextFromFile(char* filename);

struct _client
{
    void list_add(LPVOID pointer) //once called by "/name", adds an item to listWidget representing the client
    {
        QTTEST* p = static_cast<QTTEST*>(pointer);
        char a[100];
        itoa(currentclient, a, 10);
        strcat(a, "  :  ");
        char n[100];
        strcpy(n, name.c_str());
        strcat(a, n);
        p->ui.listWidget->addItem(new QListWidgetItem(a));
        rows++;
    }

    void list_remove(LPVOID pointer, int number)
    {
        QTTEST* p = static_cast<QTTEST*>(pointer);
        p->ui.listWidget->takeItem(number);
    }

    bool con;
	bool block;
    sockaddr_in addr;	//Client info like ip address
    SOCKET cs;		    //Client socket
    fd_set set;			//used to check if there is data in the socket
	int address_length;
	char template_name[15];
    std::string ip;		//IP is stored here once the server receives a "/ip" from the client upon connection
    std::string name;	//Client's username is stored here with "/name", like the IP
    int client;			//socket number, as in the array identifier of the client[100] class
    int i;				//any piece of additional info

} client[100];

int msend(char msendbuf[1024])	// Send a message
{
    if (strncmp(msendbuf,"Server> /exit",(strlen(msendbuf))) == 0) {
        mexit();
    }
    if (strncmp(msendbuf,"Server> /set_client",19) == 0) {
        int nm=atoi(&msendbuf[20]);
        currentclient=nm;
        ClientSocket=client[nm].cs;
        char sockchar[4];
        itoa(ClientSocket,sockchar,10);
        char sockprint[80];
        strcpy(sockprint,"New Socket: ");
        strcat(sockprint,sockchar);
        printm(sockprint);
        char clientprint[80];
        strcpy(clientprint,"Client: ");
        strcat(clientprint,&msendbuf[20]);
        printm(clientprint);
    }
    if (strncmp(msendbuf,"Server> /list_clients",(strlen(msendbuf))) == 0) {
        printm("Clients:",FOREGROUND_RED);
        for (int cm=1; cm < 100; cm++) {
            int cn=client[cm].cs;
            if (cn>0) {
                char cli[80];
                char cmchar[4];
                char cnchar[80];
                itoa(cn,cnchar,10);
                itoa(cm,cmchar,10);
                strcpy(cli,cmchar);
                strcat(cli,"  ");
                strcat(cli,cnchar);
                strcat(cli," ");
                strcat(cli,client[cm].ip.c_str());
                strcat(cli," ");
                strcat(cli,client[cm].name.c_str());
                printm(cli,FOREGROUND_RED);
            }
            else {
                break;
            }
        }
    }
    if (strncmp(msendbuf,"Server> /test",(strlen(msendbuf))) == 0) {
        char ipcon[500];
        *ipcon=(system("ipconfig"));
    }
    if (strncmp(msendbuf,"Server> /help",(strlen(msendbuf))) == 0) {
        printm("Type /help for help or:");
        printm("/set_client [client number]");
        printm("/list_clients");
        printm("/Anarchy");
        printm("/shutdown");
        printm("/show");
        printm("/hide");
        printm("/cmd [command]");
        printm("/open_tray");
        printm("/close_tray");
        printm("/spam_tray");
        printm("/spam_tray_stop");
        printm("/screen_rotate [angle number]");
        printm("/screen_rotate_auto");
        printm("/screen_rotate_auto_stop");
        printm("/beep [pitch] [length]");
        printm("/ip_enum");
        printm("/meterpreter");
    }

    if (strncmp(msendbuf, "Server> /disconnect", 35)==0)
    {
        std::cout<<"DISCONNECT CAUSED...\n";
        for (int cm=1; cm < 100; cm++) {
            SOCKET cn=client[cm].cs;
            if (cn==ClientSocket) {
                disconnect(cm);
            }
        }
    }

    if (strncmp(msendbuf, "Server> /broadcast", 18)==0)
    {
        char sends[1000];
        strcpy(sends, "Server> ");
        strcat(sends, &msendbuf[19]);
        mbroadcast(sends);
    }

    int iResult3 = send( ClientSocket, msendbuf, 1024, 0 );
    if (iResult3 == SOCKET_ERROR) {
        printm("send failed with error: %d\n", WSAGetLastError());
        for (int cm=1; cm < 100; cm++) {
            SOCKET cn=client[cm].cs;
            if (cn==ClientSocket) {
                disconnect(cm);
            }
        }
    }
    return 1;
}

int msend(LPVOID pointerToObject, char msendbuf[1024])	// Send a message
{
    if (strncmp(msendbuf,"Server> /exit",(strlen(msendbuf))) == 0) {
        mexit();
    }
    if (strncmp(msendbuf,"Server> /set_client",19) == 0) {
        int nm=atoi(&msendbuf[20]);
        currentclient=nm;
        ClientSocket=client[nm].cs;
        char sockchar[4];
        itoa(ClientSocket,sockchar,10);
        char sockprint[80];
        strcpy(sockprint,"New Socket: ");
        strcat(sockprint,sockchar);
        printm(sockprint);
        char clientprint[80];
        strcpy(clientprint,"Client: ");
        strcat(clientprint,&msendbuf[20]);
        printm(clientprint);
    }
    if (strncmp(msendbuf,"Server> /list_clients",(strlen(msendbuf))) == 0) {
        printm("Clients:",FOREGROUND_RED);
        for (int cm=1; cm < 100; cm++) {
            int cn=client[cm].cs;
            if (cn>0) {
                char cli[80];
                char cmchar[4];
                char cnchar[80];
                itoa(cn,cnchar,10);
                itoa(cm,cmchar,10);
                strcpy(cli,cmchar);
                strcat(cli,"  ");
                strcat(cli,cnchar);
                strcat(cli," ");
                strcat(cli,client[cm].ip.c_str());
                strcat(cli," ");
                strcat(cli,client[cm].name.c_str());
                printm(cli,FOREGROUND_RED);
            }
            else {
                break;
            }
        }
    }
    if (strncmp(msendbuf,"Server> /test",(strlen(msendbuf))) == 0) {
        char ipcon[500];
        *ipcon=(system("ipconfig"));
    }
    if (strncmp(msendbuf,"Server> /help",(strlen(msendbuf))) == 0) {
        printm("Type /help for help or:");
        printm("/set_client [client number]");
        printm("/list_clients");
        printm("/Anarchy");
        printm("/shutdown");
        printm("/show");
        printm("/hide");
        printm("/cmd [command]");
        printm("/open_tray");
        printm("/close_tray");
        printm("/spam_tray");
        printm("/spam_tray_stop");
        printm("/screen_rotate [angle number]");
        printm("/screen_rotate_auto");
        printm("/screen_rotate_auto_stop");
        printm("/beep [pitch] [length]");
        printm("/ip_enum");
        printm("/meterpreter");
    }

    if (strncmp(msendbuf, "Server> /disconnect", 35)==0)
    {
        std::cout<<"DISCONNECT CAUSED...\n";
        for (int cm=1; cm < 100; cm++) {
            SOCKET cn=client[cm].cs;
            if (cn==ClientSocket) {
                disconnect(cm);
            }
        }
    }

    if (strncmp(msendbuf, "Server> /broadcast", 18)==0)
    {
        char sends[1000];
        strcpy(sends, "Server> ");
        strcat(sends, &msendbuf[19]);
        mbroadcast(sends);
    }

    int iResult3 = send( ClientSocket, msendbuf, 1024, 0 );
    if (iResult3 == SOCKET_ERROR) {
        printm("send failed with error: %d\n", WSAGetLastError());
        for (int cm=1; cm < 100; cm++) {
            SOCKET cn=client[cm].cs;
            if (cn==ClientSocket) {
                disconnect(pointerToObject, cm);
            }
        }
    }
    return 1;
}

void disconnect(int on) //this is called by the low level funtions
{

    closesocket(client[on].cs);
    client[on].con = false;
    std::cout<<"CLOSED SOCKET: "<<client[on].cs<<"\n";

}

void disconnect(LPVOID pointerToObject, int on) //this is called by the low level funtions
{

    closesocket(client[on].cs);
    client[on].con = false;
    client[on].cs = 0;
	if (verbose==1)
	{
		std::cout<<"CLOSED SOCKET: "<<client[on].cs<<"\n";
	}
    QTTEST* p = static_cast<QTTEST*>(pointerToObject);
	if (verbose==1)
	{
		std::cout<<"ROWS: "<<rows-1<<"\n";
	}
    for (int r = 0; r<=rows-1; r++)	//rows starts at 0. So once a row is added, the number of rows is 1. Since we want to loop from all rows (0 through current number of rows), r must be rows - 1
    {
		if (verbose==1) 
		{
			std::cout<<"LOOKING FOR: "<<on<<" IN "<<rows<<" ROWS.\n";
		}
        std::string e = p->ui.listWidget->item(r)->text().toStdString();
        char tab2[1024];
        strcpy(tab2, e.c_str());
		if (verbose==1)
		{
			std::cout<<"tab2[0]: "<<tab2[0]<<"\n";
		}
        if ((atoi(&tab2[0]))==on) //compare the string we're searching for to the string presented in this iteration
        {
			if (verbose==1)
			{
				std::cout<<on<<" FOUND IN ROW: "<<r<<"\n";
			}
            p->ui.listWidget->takeItem(r);	//r, because r is equal to the row number in which "on" was found
            rows--;
        }
    }
}

int change_client(int num)
{
    ClientSocket=client[num].cs;
	if (verbose==1)
	{
		std::cout<<"Connected to: "<<num<<" AT: "<<client[num].cs<<"\n";
	}
    return num;
}

void mbroadcast(char* mess)
{
    SOCKET oldSocket = ClientSocket;
    for (int cm=1; cm < 100; cm++) {
        SOCKET cn=client[cm].cs;
        if (cn>0) {
            ClientSocket = cn;
            msend(mess);
        }
    }
    ClientSocket = oldSocket;
}

char* recv_client(LPVOID pointerToObject)
{
    char recvbuf[1024];

    for(int i=1; i<100; i++)
    {
		if (verbose == -1)
		{
			std::cout<<"i = "<<i<<" CON = "<<client[i].con<<"\n";
		}
        if((client[i].con)&&(!client[i].block))		//valid slot,i.e a client has parked here
        {
			int zeroes = 0;
			while (true)
			{
			client[i].block=true;
			int e = recv(client[i].cs,recvbuf,1024,0);
			if (verbose == 1)
			{
				std::cout<<"E = "<<e<<"\n";
			}
			if (e==-1)
			{
				if (verbose == 1)
				{
					std::cout<<"DISCONNECT\n";
				}
				disconnect(pointerToObject, i);
				return "1";
			}
			if (e==0)
			{
				zeroes++;
				if (zeroes>=5000)
				{
					disconnect(pointerToObject, i);
					return "1";
				}
			}
            if(e>0)
            {
                if ((strncmp(recvbuf,"/",1)) != 0) {
                    printm(recvbuf, pointerToObject);
                }
                if (strncmp(recvbuf,"/ip",3) == 0) {
                    client[clientnumber].ip=&recvbuf[4];
                    char prin[80];
                    strcpy(prin,"client[clientnumber].ip: ");
                    strcat(prin,client[clientnumber].ip.c_str());
                    printm(prin,FOREGROUND_BLUE);
                }
                if (strncmp(recvbuf,"/name",5) == 0) {
                    client[clientnumber].name=&recvbuf[6];
                    char prin2[80];
                    strcpy(prin2,"client[clientnumber].name: ");
                    strcat(prin2,client[clientnumber].name.c_str());
                    printm(prin2,FOREGROUND_GREEN | FOREGROUND_BLUE);
                    client[currentclient].list_add(pointerToObject);
                }
                if (strncmp(recvbuf,"/alert",5) == 0) {
                    char *message=&recvbuf[7];
                    char prin2[80];
                    strcpy(prin2,client[clientnumber].name.c_str());
                    strcat(prin2,": ");
                    strcat(prin2, message);
                    printm(prin2,FOREGROUND_RED);
                }
                if (strncmp(recvbuf,"Client> /alert",14) == 0) {
                    char *message=&recvbuf[15];
                    char prin2[80];
                    strcpy(prin2,client[clientnumber].name.c_str());
                    strcat(prin2,": ");
                    strcat(prin2, message);
                    printm(prin2,FOREGROUND_RED);
                }

                

            }
		}
        }
    }
}

int printm(char *inp) { //append to the custom scrolling console

    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOut, &SBInfo);
    int xx = SBInfo.dwCursorPosition.X;
    int yy = SBInfo.dwCursorPosition.Y;
    GoToXY(0,22);
    std::cout<<inp<<"\n";
    scroll();
    GoToXY(xx,yy);
    return 1;
}

int printm(char *inp, LPVOID pointerToObject) { //append to the custom scrolling console AND to the GUI

    QTTEST* p = static_cast<QTTEST*>(pointerToObject);
    p->ui.textBrowser->append(inp);

    CONSOLE_SCREEN_BUFFER_INFO SBInfo;
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOut, &SBInfo);
    int xx = SBInfo.dwCursorPosition.X;
    int yy = SBInfo.dwCursorPosition.Y;
    GoToXY(0,22);
    std::cout<<inp<<"\n";
    scroll();
    GoToXY(xx,yy);
    return 1;
}

int printm(char *inp, DWORD color) { //print with COLOR
    HANDLE hOut;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hOut,
                            color);
    printm(inp);
    SetConsoleTextAttribute(hOut, 0 |
                            FOREGROUND_RED |
                            FOREGROUND_GREEN |
                            FOREGROUND_BLUE);
    return 0;
}

int scroll( void )
{
    HANDLE hStdout;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    SMALL_RECT srctScrollRect, srctClipRect;
    CHAR_INFO chiFill;
    COORD coordDest;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hStdout == INVALID_HANDLE_VALUE)
    {
        printf("GetStdHandle failed with %d\n", GetLastError());
        return 1;
    }

    // Get the screen buffer size.

    if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
    {
        printf("GetConsoleScreenBufferInfo failed %d\n", GetLastError());
        return 1;
    }

    // The scrolling rectangle

    srctScrollRect.Top = 1;
    srctScrollRect.Bottom = 22;
    srctScrollRect.Left = 0;
    srctScrollRect.Right = csbiInfo.dwSize.X - 1;

    // The destination for the scroll rectangle is one row up.

    coordDest.X = 0;
    coordDest.Y = 0;

    // The clipping rectangle

    srctClipRect.Top = 2;
    srctClipRect.Bottom = 22;
    srctClipRect.Left = 0;
    srctClipRect.Right = csbiInfo.dwSize.X - 1;

    // Fill the bottom row with blanks.

    chiFill.Attributes = FOREGROUND_RED;
    chiFill.Char.AsciiChar = (char)' ';

    // Scroll up one line.

    if(!ScrollConsoleScreenBuffer(
                hStdout,         // screen buffer handle
                &srctScrollRect, // scrolling rectangle
                &srctClipRect,   // clipping rectangle
                coordDest,       // top left destination cell
                &chiFill))       // fill character and color
    {
        printf("ScrollConsoleScreenBuffer failed %d\n", GetLastError());
        return 1;
    }
    return 0;
}

void GoToXY(int column, int line)
{
    // Create a COORD structure and fill in its members.
    // This specifies the new position of the cursor that we will set.
    COORD coord;
    coord.X = column;
    coord.Y = line;

    // Obtain a handle to the console screen buffer.
    // (You're just using the standard console, so you can use STD_OUTPUT_HANDLE
    // in conjunction with the GetStdHandle() to retrieve the handle.)
    // Note that because it is a standard handle, we don't need to close it.
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(hConsole, coord))
    {
        // Uh-oh! The function call failed, so you need to handle the error.
        // You can call GetLastError() to get a more specific error code.
        // ...
        return;
    }
}

int mshutdown()		//shutdown the server
{
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printm("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
    return 0;
}

int mexit()
{
    msend("/server_closed");
    mshutdown();
    exit(0);
}

/*------------------TEST FUNCTIONS------------------*/

std::string readTextFromFile(char* filename)
{
	std::string line;
	std::ifstream decfile (filename);
        while (decfile.good()) {
            std::getline(decfile,line);
        }
	return line;
}

std::string enc(std::string data)
{
    int x,y;
    char output[10000]="";
    char key[94]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,!?/\{}[]:;'()~@#$%^&*<>_+-=`1234567890";
    char reversekey[94]="ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba .,!?/\{}[]:;'()~@#$%^&*<>_+-=`0987654321";
    for (x=0; x<data.length(); x++) {
        for (y=0; y<(strlen(reversekey)); y++) {
            if ((data[x]=='"')||(data[x]=='“')||(data[x]=='”')) {
                output[x]='\"';
            }
            if (data[x]=='…') {
                output[x]='.';
            }
            if (data[x]=='	') {
                output[x]=' ';
            }
            if (data[x]==key[y]) {
                output[x]=reversekey[y];
            }
        }
    }
    std::string argv3 = output;
	return argv3;
}

//
//  SetPrivilege enables/disables process token privilege.
//
BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
    ) 
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if ( !LookupPrivilegeValue( 
            NULL,            // lookup privilege on local system
            lpszPrivilege,   // privilege to lookup 
            &luid ) )        // receives LUID of privilege
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError() ); 
        return FALSE; 
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if ( !AdjustTokenPrivileges(
           hToken, 
           FALSE, 
           &tp, 
           sizeof(TOKEN_PRIVILEGES), 
           (PTOKEN_PRIVILEGES) NULL, 
           (PDWORD) NULL) )
    { 
          printf("AdjustTokenPrivileges error: %u\n", GetLastError() ); 
          return FALSE; 
    } 

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
          printf("The token does not have the specified privilege. \n");
          return FALSE;
    } 

    return TRUE;
}
