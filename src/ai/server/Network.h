#pragma once

#include <common/Compiler.h>
#include <string>
#include <stdint.h>
#include <list>
#include <deque>
#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2spi.h>
#else
#define SOCKET  int
#include <sys/select.h>
#endif

namespace ai {

class IProtocolMessage;
typedef uint8_t ClientId;
typedef std::deque<char> streamContainer;

class Network {
protected:
	int _port;
	std::string _hostname;
	// the socket file descriptor
	SOCKET _socketFD;
	SOCKET _maxFD;
	fd_set _readFDSet;
	fd_set _writeFDSet;

	struct Client {
		Client(SOCKET _socket) :
				socket(_socket) {
			finished = false;
		}
		SOCKET socket;
		bool finished;
		streamContainer in;
		streamContainer out;
	};

	typedef std::list<Client> ClientSockets;
	typedef ClientSockets::iterator ClientSocketsIter;
	ClientSockets _clientSockets;
	ClientSocketsIter closeClient (ClientSocketsIter& i);
public:
	Network(int port = 10001, const std::string& hostname = "0.0.0.0");
	virtual ~Network();

	bool start();
	void update(uint32_t deltaTime);

	void broadcast(const IProtocolMessage& msg);
};

}
