#include "Server.h"


Server::Server(int serverPORT,bool broadcastPublically)
{
	WORD sockVersion = MAKEWORD(2, 1);
	WSADATA wsadata;
	
	if (WSAStartup(sockVersion, &wsadata) != 0)
	{
		printf("WSAStartup failed with error: %d\n");
		return;
	}
	sin.sin_family = AF_INET;
	sin.sin_port = htons(serverPORT);
	if (broadcastPublically)
		sin.sin_addr.S_un.S_addr= htonl(INADDR_ANY);
	else
		sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	slisten = socket(AF_INET, SOCK_STREAM, NULL);
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "bind error" << endl;
		return;
	}
	cout << "bind socket succeed!" << endl;
	if (listen(slisten, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "listening error" << endl;
		return;
	}
	cout << "start listening..." << endl;
	serverPtr = this;
}
bool Server::listenForNewConnection()
{
	SOCKET newConnection;
	int connectionCounter = 0;
	newConnection = accept(slisten, (SOCKADDR *)&sin, &sinlen);
	if (newConnection == INVALID_SOCKET)
	{
		printf("accept failed with error:%d", WSAGetLastError());
		return false;
	}
	else {
		cout << "Client Connected!" << endl;
		connections[totalConnections] = newConnection;		
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(totalConnections), NULL, NULL);
		string MOTD = "Welcome!";
		sendString(totalConnections, MOTD);
		totalConnections += 1;
		return true;
	}

}

bool Server::processPacket(int ID,packet packettype)
{
	switch (packettype)
	{
	case P_chatMessage:
	{
		string message;
		if (!getString(ID, message))
			return false;
		for (int i = 0;i < totalConnections;++i) {
			if (i == ID)
				continue;
			if (!sendString(i, message))
			{
				cout << "Failed to send message from client ID:" << ID << endl;
			}
		}
		break;
	}
	default:
		cout << "Unrecognized packet:" << packettype << endl;
		break;
	}
	return true;
}
void Server::ClientHandlerThread(int ID)
{
	packet packetType;
	while (true)
	{
		if (!serverPtr->getPacketType(ID, packetType))
			break;
		if (!serverPtr->processPacket(ID, packetType))
			break;
	}
	cout << "Lost connection to client:"<< ID << endl;
}
