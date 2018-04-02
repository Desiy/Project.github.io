#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Client.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 44255

using namespace std;

void Client::startup()
{
	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	sockaddr_in seraddr;
	char data[DEFAULT_BUFLEN];
	const char *sendbufptr = data;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}
	while (true) {
		SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sClient == INVALID_SOCKET) {
			cout << "Invalid Socket!" << endl;
			return;
		}

		seraddr.sin_port = htons(DEFAULT_PORT);
		seraddr.sin_family = AF_INET;
		seraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		if(connect(sClient, (sockaddr*)&seraddr, sizeof(seraddr)) == SOCKET_ERROR) {
			cout << "connect failed!" << endl;
			cout << WSAGetLastError() << endl;
			closesocket(ConnectSocket);
			WSACleanup();
			return;
		}
		cin >> data;
		send(sClient, sendbufptr, strlen(sendbufptr), 0);
		iResult = recv(sClient, recvbuf, sizeof(recvbuf), 0);
		if (iResult > 0) {
			printf(recvbuf);
		}
		closesocket(sClient);
	}
	WSACleanup();
	
	return;

}