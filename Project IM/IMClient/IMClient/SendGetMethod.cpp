#include "Client.h"

bool Client::sendAll(char *data, int totalBytes)
{
	int bytesSend = 0;
	while (bytesSend<totalBytes)
	{
		int rentCheck = send(connection, data+bytesSend, totalBytes - bytesSend, NULL);
		if (rentCheck == SOCKET_ERROR)
			return false;
		bytesSend += rentCheck;
	}
	return true;
}

bool Client::sendPacketType(packet _packetType)
{
	if(!sendInt32_t(_packetType))
		return false;
	return true;
}

bool Client::sendInt32_t(int32_t _int32_t)
{
	_int32_t = htonl(_int32_t);
	if (!sendAll((char*)&_int32_t, sizeof(_int32_t)))
		return false;
	return true;
}

bool Client::sendString(string &_string)
{
	if (!sendPacketType(P_chatMessage))
		return false;
	int32_t bufferlen = _string.size();
	if (!sendInt32_t(bufferlen))
		return false;
	if (!sendAll((char*)_string.c_str(), bufferlen))
		return false;
	return true;
}




bool Client::recvAll(char *data, int totalBytes)
{
	int bytesReceived = 0;
	while (bytesReceived < totalBytes) {
		int rentCheck = recv(connection, data + bytesReceived, totalBytes - bytesReceived, NULL);
		if (rentCheck == SOCKET_ERROR)
			return false;
		bytesReceived += rentCheck;
	}
	return true;
}

bool Client::getPacketType(packet &_packetType)
{
	int packettype = _packetType;
	if (!getInt32_t(packettype))
		return false;
	_packetType = (packet)packettype;
	return true;
}

bool Client::getInt32_t(int32_t &_int32_t)
{
	if (!recvAll((char*)&_int32_t, sizeof(_int32_t)))
		return false;
	_int32_t = ntohl(_int32_t);
	return true;
}


bool Client::getString(string &_string)
{
	int32_t bufferlen;
	if (!getInt32_t(bufferlen))
		return false;
	char* buffer = new char[bufferlen + 1];
	buffer[bufferlen] = '\0';
	if (!recvAll(buffer, bufferlen)) {
		delete[]buffer;
		return false;
	}
	_string = buffer;
	delete[]buffer;

	cout << _string << endl;
	return true;
}