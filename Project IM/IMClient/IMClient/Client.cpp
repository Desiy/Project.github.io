#include "Client.h"


Client::Client(string IP,int PORT)
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("WSAStartup failed with error: %d\n");
		exit(0);
	}
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP.c_str());
	clientPtr = this;
}

bool Client::Connect()
{
	connection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (connect(connection,(sockaddr*)&addr,sizeof(addr))!=0) {
		cout << "connect failed!" << endl;
		return false;
	}
	cout << "connected!" << endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientThread, NULL, NULL, NULL);
	return true;
}

bool Client::processPacket(packet packettype)
{
	switch (packettype)
	{
	case P_chatMessage:
	{
		string message;
		if (!getString(message))
			return false;
		break;
	}
	default:
		cout << "Unrecognized packet:" << packettype << endl;
		break;
	}
	return true;
}


bool Client::closeConnection()
{
	if (closesocket(connection) == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAENOTSOCK)
			return true;
		string errorMessage = "Failed to close the socket.Winsock error:" + to_string(WSAGetLastError())+".";
		return false;
	}
	return true;
}

void Client::clientThread()
{
	packet packetType;
	while (true)
	{
		if (!clientPtr->getPacketType(packetType))
			break;
		if (!clientPtr->processPacket(packetType))
			break;
	}
	cout << "Lost connection to server." << endl;
	if (clientPtr->closeConnection())
	{
		cout << "Socket to server was close successfully." << endl;
	}
	else
	{
		cout << "Socket was unable to close." << endl;
	}
}

bool Client::requestFile(string fileName)
{
	
}
