#pragma comment(lib,"ws2_32.lib") 

#include <winsock2.h>
#include <string>
#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS


#define BUFFLEN 215

using namespace std;

enum packet
{
	P_chatMessage,
	P_fileTransferRequestFile,
	P_fileTransfer_EndOfFile,
	P_fileTranferByteBuffer,
	P_fileTransferRequestNextFile
};
class Client
{
public:
	Client(string IP, int port);
	bool Connect();
	bool closeConnection();
	bool sendString(string &_string);
	bool requestFile(string fileName);
private:
	bool processPacket(packet packettype);
	static void clientThread();


	bool sendAll(char* data,int totalBytes);
	bool sendInt32_t(int32_t _int32_t);
	bool sendPacketType(packet _packetType);
	
	bool recvAll(char *data, int totalBytes);
	bool getInt32_t(int32_t &_int32_t);
	bool getPacketType(packet &_packetType);
	bool getString(string &_string);

private:
	SOCKET connection = INVALID_SOCKET;
	sockaddr_in addr;
};
static Client* clientPtr;