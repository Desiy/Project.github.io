#pragma comment (lib, "Ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>

using namespace std;

#define BUFFLEN 215



enum packet
{
	P_chatMessage
};
class Server
{
public:
	Server(int serverPORT, bool broadcastPublically = false);
	bool listenForNewConnection();
private:
	bool sendAll(int ID,char* data, int totalBytes);
	bool sendPacketType(int ID, packet _packetType);
	bool sendInt32_t(int ID, int32_t _int32_t);
	bool sendString(int ID, string &_string);
	
	bool recvAll(int ID,char *data, int totalBytes);
	bool getPacketType(int ID, packet &_packetType);
	bool getInt32_t(int ID, int32_t &_int32_t);
	bool getString(int ID, string &_string);

	bool processPacket(int ID, packet packettype);

	static void ClientHandlerThread(int ID);

private:
	SOCKET connections[100];
	int totalConnections = 0;
	sockaddr_in sin;
	int sinlen = sizeof(sin);
	SOCKET slisten;
};
static Server *serverPtr;