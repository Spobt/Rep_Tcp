// TcpServ.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <assert.h>
#include <conio.h>
#pragma comment (lib, "ws2_32.lib")

#define BACKLOG 10

// SOCKET就是一个int,他对应着SOCKADD,所以 ...

int main(int argc, char* argv[])
{
	// 0. ::WSAStartup
	WSADATA wsaData;
	int iRes_WSAStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != iRes_WSAStartup) { assert(0); }

	// 1. ::socket
	SOCKET sockServ = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sockServ) { assert(0); }

	// 2. ::bind (SOCKET与SOCKADDR的对应关系的建立)
	SOCKADDR_IN addrServ;
	addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(6666);
	memset(addrServ.sin_zero, 0, sizeof(CHAR[8]));
	int iRes_bind = bind(sockServ, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));
	if (0 != iRes_bind) { assert(0); }

	// 3. ::listen
	int iRes_listen = listen(sockServ, BACKLOG);
	if (0 != iRes_listen) { assert(0); }

	// 4. Loop
	SOCKADDR_IN addrClnt;
	int iLen = sizeof(SOCKADDR);
	while (1)
	{
		// 4-0. ::accept (SOCKET与SOCKADDR的对应关系的建立)
		SOCKET sockConn = accept(sockServ, (SOCKADDR*)&addrClnt, &iLen);
		if (INVALID_SOCKET == sockConn) { assert(0); }

		static int s_iNum = 0;		// 文件名的尾数,防止名字相同 & 连接次数

		// 4-1. ::send
		char strSend[0xff] = "";
		sprintf_s(strSend, sizeof(strSend), "Server: Hello , how are you ! \n这是第%d个来访者 !\n", s_iNum);
		int iLen = strlen(strSend) + 1;
		int iRes_send = send(sockConn, strSend, iLen, 0);
		if (SOCKET_ERROR == iRes_send) { assert(0); }	// 发生错误
		if (iLen != iRes_send) { assert(0); }	// 发送的数据不完整

		// 4-2. ::recv
		char strFileName[0xff];		// 文件名
		FILE* pFile = NULL;			// 文件流
		memset(strFileName, 0, sizeof(strFileName));
		// 创建不重名的文件名
		sprintf_s(strFileName, sizeof(strFileName), "file_%d.png", s_iNum);
		s_iNum++;
		// 创建并打开文件
		int iRes_fipen_s = fopen_s(&pFile, strFileName, "wb");
		if (0 != iRes_fipen_s || NULL == pFile) { assert(0); }
		//
		while (1)
		{
			char strRecv[0x400];
			memset(strRecv, 0, sizeof(strRecv));
			int iRes_recv = recv(sockConn, strRecv, sizeof(strRecv), 0);
			if (SOCKET_ERROR == iRes_recv) { assert(0); }	// 发生错误
			if (iRes_recv < 0x400)
			{
				// 接收完成
				fwrite(strRecv, iRes_recv, 1, pFile);
				fclose(pFile);
				break;
			}
			else if (0x400 == iRes_recv)
			{
				// 下次还要接收 这里是有点bug的(如果数据就是0x400)
				fwrite(strRecv, iRes_recv, 1, pFile);
			}
		}

		// 4-3. ::closesocket
		closesocket(sockConn);
	}

	closesocket(sockServ);
	WSACleanup();
	system("pause");

	return 0;
}