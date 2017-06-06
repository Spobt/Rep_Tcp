// UdpClnt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <assert.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	// 0. ::WSAStartup
	// 2. ::socket
	// 3. ::bind
	// 4. ::sendto
	// 5. ::recvfrom
	// 6. closeall

	// 0. ::WSAStartup
	WSADATA wsaData;
	int iRes_WSAStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != iRes_WSAStartup) { assert(0); }

	// 1. ::socket
	SOCKET sockClnt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sockClnt) { assert(0); }

	// 2. ::bind (SOCKET与SOCKADDR的对应关系的建立)
	SOCKADDR_IN addrServ;
	int iLen = sizeof(SOCKADDR);
	addrServ.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(6667);
	memset(addrServ.sin_zero, 0, sizeof(CHAR[8]));
	//int iRes_bind = bind(sockServ, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));
	//if (0 != iRes_bind) { assert(0); }
	printf("客户端启动成功! \n");

	// 4. ::sendto
	char strSend[] = "你好, 我是客户端 ! \n";
	sendto(sockClnt, strSend, sizeof(strSend), 0, (SOCKADDR*)&addrServ, iLen);

	// 5. ::recvfrom
	SOCKADDR_IN addrFrom;
	int iLenFrom = sizeof(SOCKADDR);
	char strRecv[0x400] = {};
	recvfrom(sockClnt, strRecv, 0x400, 0, (SOCKADDR*)&addrFrom, &iLenFrom);
	printf("%s", strRecv);

	// 6. closeall
	closesocket(sockClnt);
	WSACleanup();
	system("pause");

    return 0;
}