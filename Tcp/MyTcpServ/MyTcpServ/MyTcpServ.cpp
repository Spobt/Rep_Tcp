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
	/*如果你的服务器有多个网卡（每个网卡上有不同的IP地址），而你的服务（不管是在udp端口上侦听，还是在tcp端口上侦听），
	出于某种原因：可能是你的服务器操作系统可能随时增减IP地址，也有可能是为了省去确定服务器上有什么网络端口（网卡）
	的麻烦 —— 可以要在调用bind()的时候，告诉操作系统：“我需要在 yyyy 端口上侦听，所有发送到服务器的这个端口，不管是
	哪个网卡 / 哪个IP地址接收到的数据，都是我处理的。”这时候，服务器程序则在0.0.0.0这个地址上进行侦听。例如：*/
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
	//int iLen = 0;		//_Inout_	这里长度不对会报错
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
		::sprintf_s(strSendMsg, sizeof(strSendMsg), "sockConn--> Welcome you(%s) come here !", strIP);	//不给长度会隐式使用模板
		int iLen = strlen(strSendMsg) + 1;
		int iResOfSend = ::send(sockConn, strSendMsg, iLen, 0);
		if (iResOfSend != iLen) { assert(0); }

		// ::recv
		char strRecvMsg[0xff]{};
		int iResOfRecv = ::recv(sockConn, strRecvMsg, sizeof(strRecvMsg), 0);	//返回 size_byte
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

