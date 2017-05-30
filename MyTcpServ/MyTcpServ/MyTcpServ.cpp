// MyTcpServ.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cassert>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define BACKLOG 5

int main()
{
	// ::WSAStartup
	WSADATA wsaData;
	int iResOfWsastartup = ::WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (0 != iResOfWsastartup) { assert(0); }

	// ::socket
	SOCKET sockServ = ::socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sockServ) { assert(0); }

	// ::bind
	/*�����ķ������ж��������ÿ���������в�ͬ��IP��ַ��������ķ��񣨲�������udp�˿���������������tcp�˿�����������
	����ĳ��ԭ�򣺿�������ķ���������ϵͳ������ʱ����IP��ַ��Ҳ�п�����Ϊ��ʡȥȷ������������ʲô����˿ڣ�������
	���鷳 ���� ����Ҫ�ڵ���bind()��ʱ�򣬸��߲���ϵͳ��������Ҫ�� yyyy �˿������������з��͵�������������˿ڣ�������
	�ĸ����� / �ĸ�IP��ַ���յ������ݣ������Ҵ���ġ�����ʱ�򣬷�������������0.0.0.0�����ַ�Ͻ������������磺*/
	SOCKADDR_IN addrServ;
	addrServ.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = ::htons(1001);
	int iResOfBind = ::bind(sockServ, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));
	if (0 != iResOfBind) { assert(0); }

	// ::listen
	int iResOfListrn = ::listen(sockServ, BACKLOG);
	if (0 != iResOfListrn) { assert(0); }

	//
	SOCKADDR_IN addrClnt;
	//int iLen = 0;		//_Inout_	���ﳤ�Ȳ��Իᱨ��
	int iLen = sizeof(SOCKADDR);
	while (true)
	{
		// ::accept
		SOCKET sockConn = ::accept(sockServ, (SOCKADDR*)&addrClnt, &iLen);
		//if (INVALID_SOCKET == sockConn) { assert(0); }

		// ::send
		//ip
		char strIP[0x40]{};
		::InetNtopA(AF_INET, &addrClnt.sin_addr, (PSTR)&strIP, sizeof(strIP));
		char strSendMsg[0xff]{};
		::sprintf_s(strSendMsg, sizeof(strSendMsg), "sockConn--> Welcome you(%s) come here !", strIP);	//�������Ȼ���ʽʹ��ģ��
		int iLen = strlen(strSendMsg) + 1;
		int iResOfSend = ::send(sockConn, strSendMsg, iLen, 0);
		if (iResOfSend != iLen) { assert(0); }

		// ::recv
		char strRecvMsg[0xff]{};
		int iResOfRecv = ::recv(sockConn, strRecvMsg, sizeof(strRecvMsg), 0);	//���� size_byte
		if (0 == iResOfRecv) { assert(0); }
		if (SOCKET_ERROR == iResOfRecv) { assert(0); }
		//output
		cout << strRecvMsg << endl;

		// ::closesocket
		::closesocket(sockConn);
	}

	::WSACleanup();

	::system("pause");

    return 0;
}

