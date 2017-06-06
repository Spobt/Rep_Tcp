// UdpServ.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <assert.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
	// 0. ::WSAStartup
	// 1. ::socket
	// 2. ::bind
	// 3. ::recvfrom
	// 4. ::sendto
	// 5. closeall

	// 0. ::WSAStartup
	WSADATA wsaData;
	int iRes_WSAStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != iRes_WSAStartup) { assert(0); }

	// 1. ::socket
	SOCKET sockServ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == sockServ) { assert(0); }

	// 2. ::bind (SOCKET与SOCKADDR的对应关系的建立)
	SOCKADDR_IN addrServ;
	addrServ.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(6667);
	memset(addrServ.sin_zero, 0, sizeof(CHAR[8]));
	int iRes_bind = bind(sockServ, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));
	if (0 != iRes_bind) { assert(0); }
	printf("服务器启动成功! \n");

	while (true)
	{
		// 3. ::recvfrom
		SOCKADDR_IN addrConn;
		int iLen = sizeof(SOCKADDR);
		char strRecv[0x400] = {};
		int iRes_recvfrom = recvfrom(sockServ, strRecv, sizeof(strRecv), 0, (SOCKADDR*)&addrConn, &iLen);
		if (SOCKET_ERROR == iRes_recvfrom) { assert(0); }
		char* strIP = inet_ntoa(addrConn.sin_addr);
		printf("有客户端连接,IP是 : %s ! \n", strIP);
		printf("%s", strRecv);

		// 4. ::sendto
		char strSend[] = ("你好,这是服务器给你发的数据 ! \n");
		sendto(sockServ, strSend, sizeof(strSend), 0, (SOCKADDR*)&addrConn, iLen);
	}


	// 5. closeall
	closesocket(sockServ);
	WSACleanup();
	system("pause");

    return 0;
}

