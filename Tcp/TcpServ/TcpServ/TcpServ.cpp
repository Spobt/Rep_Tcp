// TcpServ.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <assert.h>
#include <conio.h>
#pragma comment (lib, "ws2_32.lib")

#define BACKLOG 10

// SOCKET����һ��int,����Ӧ��SOCKADD,���� ...

int main(int argc, char* argv[])
{
	// 0. ::WSAStartup
	WSADATA wsaData;
	int iRes_WSAStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != iRes_WSAStartup) { assert(0); }

	// 1. ::socket
	SOCKET sockServ = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sockServ) { assert(0); }

	// 2. ::bind (SOCKET��SOCKADDR�Ķ�Ӧ��ϵ�Ľ���)
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
		// 4-0. ::accept (SOCKET��SOCKADDR�Ķ�Ӧ��ϵ�Ľ���)
		SOCKET sockConn = accept(sockServ, (SOCKADDR*)&addrClnt, &iLen);
		if (INVALID_SOCKET == sockConn) { assert(0); }

		static int s_iNum = 0;		// �ļ�����β��,��ֹ������ͬ & ���Ӵ���

		// 4-1. ::send
		char strSend[0xff] = "";
		sprintf_s(strSend, sizeof(strSend), "Server: Hello , how are you ! \n���ǵ�%d�������� !\n", s_iNum);
		int iLen = strlen(strSend) + 1;
		int iRes_send = send(sockConn, strSend, iLen, 0);
		if (SOCKET_ERROR == iRes_send) { assert(0); }	// ��������
		if (iLen != iRes_send) { assert(0); }	// ���͵����ݲ�����

		// 4-2. ::recv
		char strFileName[0xff];		// �ļ���
		FILE* pFile = NULL;			// �ļ���
		memset(strFileName, 0, sizeof(strFileName));
		// �������������ļ���
		sprintf_s(strFileName, sizeof(strFileName), "file_%d.png", s_iNum);
		s_iNum++;
		// ���������ļ�
		int iRes_fipen_s = fopen_s(&pFile, strFileName, "wb");
		if (0 != iRes_fipen_s || NULL == pFile) { assert(0); }
		//
		while (1)
		{
			char strRecv[0x400];
			memset(strRecv, 0, sizeof(strRecv));
			int iRes_recv = recv(sockConn, strRecv, sizeof(strRecv), 0);
			if (SOCKET_ERROR == iRes_recv) { assert(0); }	// ��������
			if (iRes_recv < 0x400)
			{
				// �������
				fwrite(strRecv, iRes_recv, 1, pFile);
				fclose(pFile);
				break;
			}
			else if (0x400 == iRes_recv)
			{
				// �´λ�Ҫ���� �������е�bug��(������ݾ���0x400)
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