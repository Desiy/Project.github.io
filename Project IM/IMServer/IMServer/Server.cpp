#pragma comment (lib, "Ws2_32.lib")
#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#include "Server.h"
#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define DEFAULT_PORT 44255
#define SOMAXNUM 5
#define BUFFLEN 215

void Server::startup()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsadata;
	SOCKET slisten = INVALID_SOCKET, sClient = INVALID_SOCKET;
	sockaddr_in remoteAddr;
	int	nAddrlen = sizeof(remoteAddr);
	int iResult = 0;
	int iSendResult;
	char revbuf[BUFFLEN];
	sockaddr_in sin;
	if (WSAStartup(sockVersion, &wsadata) != 0)
	{
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		cout << "socket error" << endl;
	}
	cout << "creat socket succeed!" << endl;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(DEFAULT_PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "bind error" << endl;
		return;
	}
	cout << "bind socket succeed!" << endl;
	if (listen(slisten, SOMAXNUM) == SOCKET_ERROR)
	{
		cout << "listening error" << endl;
		return;
	}
	cout << "start listening..." << endl;
	while (true) {
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept failed with error:%d", WSAGetLastError());
			continue;
		}
		printf("get a connection：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		iResult = recv(sClient, revbuf, BUFFLEN, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			iSendResult = send(sClient, revbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(sClient);
				WSACleanup();
				return;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(sClient);
			WSACleanup();
			return;
		}
	}
	iResult = shutdown(sClient, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(sClient);
		WSACleanup();
		return;
	}
	closesocket(sClient);
	WSACleanup();
	return;
}