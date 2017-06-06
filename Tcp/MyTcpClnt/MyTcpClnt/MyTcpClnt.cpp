// MyTcpClnt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main()
{
	// ::WSAStartup
	WSADATA wsaData;
	int iResOfWsastartup = ::WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (0 != iResOfWsastartup) { assert(0); }

	// ::socket
	SOCKET sockClnt = ::socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sockClnt) { assert(0); }

	// ::connect
	SOCKADDR_IN addrServ;
	addrServ.sin_addr.S_un.S_addr = ::inet_addr("127.0.0.1");
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(1001);
	int iResOfConnect = ::connect(sockClnt, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));
	if (0 != iResOfConnect) { assert(0); }
	// output
	char strIP[0x40]{};
	::InetNtopA(AF_INET, &addrServ.sin_addr, (PSTR)&strIP, sizeof(strIP));
	char strOutput[0xff]{};
	::sprintf_s(strOutput, "sockClnt--> I has connect to (%s) !", strIP);
	cout << strOutput << endl;

	// ::send
	char strSendMsg[0xff] = "sockClnt--> I`m sockClnt !";
	int iLen = strlen(strSendMsg) + 1;
	int iResOfSend = ::send(sockClnt, strSendMsg, iLen, 0);
	if (iResOfSend != iLen) { assert(0); }

	// ::recv
	char strRecvMsg[0xff]{};
	int iResOfRecv = ::recv(sockClnt, strRecvMsg, sizeof(strRecvMsg), 0);	//·µ»Ø size_byte
	if (0 == iResOfRecv) { assert(0); }
	if (SOCKET_ERROR == iResOfRecv) { assert(0); }
	//output
	cout << strRecvMsg << endl;

	//::system("pause");

	// ::closesocket
	::closesocket(sockClnt);
	::WSACleanup();

	::system("pause");


    return 0;
}

