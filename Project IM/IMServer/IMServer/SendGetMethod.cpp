#include "Server.h"

bool Server::sendAll(int ID, char* data, int totalBytes)
{
	int bytesSend = 0;
	while (bytesSend<totalBytes)
	{
		int rentCheck = send(connections[ID], data + bytesSend, totalBytes - bytesSend, NULL);
		if (rentCheck == SOCKET_ERROR)
			return false;
		bytesSend += rentCheck;
	}
	return true;
}

bool Server::sendPacketType(int ID, packet _packetType)
{
	if(!sendInt32_t(ID,_packetType))
		return false;
	return true;
}

bool Server::sendInt32_t(int ID, int32_t _int32_t)
{
	_int32_t = htonl(_int32_t);
	if (!sendAll(ID, (char*)&_int32_t, sizeof(_int32_t)))
		return false;
	return true;
}

bool Server::sendString(int ID, string &_string)
{
	if (!sendPacketType(ID, P_chatMessage))
		return false;
	int32_t bufferlen = _string.size();
	if (!sendInt32_t(ID, bufferlen))
		return false;
	if(!sendAll(ID,(char*)_string.c_str(),bufferlen))
		return false;
	return true;
}


bool Server::recvAll(int ID, char *data, int totalBytes)
{
	int bytesReceived = 0;
	while (bytesReceived < totalBytes) {
		int rentCheck = recv(connections[ID], data + bytesReceived, totalBytes - bytesReceived, NULL);
		if (rentCheck == SOCKET_ERROR)
			return false;
		bytesReceived += rentCheck;
	}
	return true;
}

bool Server::getPacketType(int ID, packet &_packetType)
{
	int packettype = _packetType;
	if (!getInt32_t(ID, packettype))
		return false;
	_packetType = (packet)packettype;
	return true;
}

bool Server::getInt32_t(int ID, int32_t &_int32_t)
{
	if(!recvAll(ID, (char*)&_int32_t, sizeof(_int32_t)))
		return false;
	_int32_t = ntohl(_int32_t);	
	return true;
}

bool Server::getString(int ID, string &_string)
{
	int32_t bufferlen;
	if (!getInt32_t(ID, bufferlen))
		return false;
	char* buffer = new char[bufferlen + 1];
	buffer[bufferlen] = '\0';
	if (!recvAll(ID, buffer, bufferlen)) {
		delete[]buffer;
		return false;
	}
	_string = buffer;
	cout << _string.c_str() << endl;
	delete[]buffer;
	return true;
}
