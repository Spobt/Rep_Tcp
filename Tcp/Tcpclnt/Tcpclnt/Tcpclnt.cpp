// Tcpclnt.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <assert.h>
#include <atlimage.h>
#include <conio.h>
#pragma comment (lib, "ws2_32.lib")


// 截屏
void ScreenShot();


int main(int argc, char* argv[])
{
	// 0. ::WSAStartup
	WSADATA wsaData;
	int iRes_WSAStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != iRes_WSAStartup) { assert(0); }

	// 1. ::socket
	SOCKET sockClnt = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sockClnt) { assert(0); }

	// ------------------------------------------
	_getch();

	// 2. ::connect
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(6666);
	addrServ.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int iRes_connect = connect(sockClnt, (SOCKADDR*)&addrServ, sizeof(SOCKADDR));
	if (0 != iRes_connect) { assert(0); }

	// 3. ::recv (这里是不严谨的)
	char strRecv[0x400];
	memset(strRecv, 0, sizeof(strRecv));
	int iRes_recv = recv(sockClnt, strRecv, sizeof(strRecv), 0);
	if (SOCKET_ERROR == iRes_recv) { assert(0); }	// 发生错误
	printf("%s", strRecv);
	Sleep(50);

	// 4. ::send
	// 截屏
	ScreenShot();
	// 发送数据的大小
	FILE* pFile = NULL;		
	fopen_s(&pFile, "send.png", "rb+");
	fseek(pFile, 0, SEEK_END);
	int iSize = ftell(pFile);
	// 发送数据
	char* strSend = (char*)malloc(iSize);
	memset(strSend, 0, iSize);
	fseek(pFile, 0, SEEK_SET);
	fread(strSend, iSize, 1, pFile);
	int iRes_send = send(sockClnt, strSend, iSize, 0);
	if (SOCKET_ERROR == iRes_send) { assert(0); }
	free(strSend);
	fclose(pFile);

	// 5. close
	remove("send.png");
	closesocket(sockClnt);
	WSACleanup();
	system("pause");

	return 0;
}

void ScreenShot()
{
	// 0. 模拟按键
	keybd_event(VK_SNAPSHOT, 0, 0, 0);
	Sleep(100);
	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0);

	// 1. 把剪切板的数据读出来 
	OpenClipboard(NULL);
	HBITMAP hBit = (HBITMAP)GetClipboardData(CF_BITMAP);
	if (NULL == hBit) {CloseClipboard(); return; }

	// 2. 保存图片
	CImage img;
	img.Attach(hBit);
	img.Save(L"send.png");
	img.Detach();

	// 关闭剪切析
	CloseClipboard();
}


