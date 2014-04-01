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
typedef std::deque<uint8_t> streamContainer;

class Network {
protected:
	uint16_t _port;
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
	void closeClient(Client& client);
public:
	Network(uint16_t port = 10001, const std::string& hostname = "0.0.0.0");
	virtual ~Network();

	bool start();
	void update(long deltaTime);

	int getConnectedClients() const;

	void broadcast(const IProtocolMessage& msg);
	bool sendToClient(Client& client, const IProtocolMessage& msg);
};

inline int Network::getConnectedClients() const {
	return static_cast<int>(_clientSockets.size());
}

}
