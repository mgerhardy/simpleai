#include "Network.h"
#include "IProtocolMessage.h"
#include "IProtocolHandler.h"
#include "ProtocolHandlerRegistry.h"
#include "ProtocolMessageFactory.h"
#ifdef WIN32
#define cleanup() WSACleanup()
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netdb.h>
#include <signal.h>
#define closesocket close
#define INVALID_SOCKET  -1
#define cleanup()
#endif
#include <string.h>
#include <deque>
#include <algorithm>
#include <cassert>

namespace ai {

Network::Network(uint16_t port, const std::string& hostname) :
		_port(port), _hostname(hostname), _socketFD(INVALID_SOCKET), _maxFD(0) {
	FD_ZERO(&_readFDSet);
	FD_ZERO(&_writeFDSet);
}

Network::~Network() {
	closesocket(_socketFD);
	cleanup();
}

bool Network::start() {
#ifdef WIN32
	WSADATA wsaData;
	const int wsaResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (wsaResult != NO_ERROR) {
		return false;
	}
#else
	signal(SIGPIPE, SIG_IGN);
#endif

	FD_ZERO(&_readFDSet);
	FD_ZERO(&_writeFDSet);

	_socketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socketFD == INVALID_SOCKET) {
		cleanup();
		return false;
	}
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(_port);

	int t = 1;
#ifdef _WIN32
	if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, (char*) &t, sizeof(t)) != 0) {
#else
	if (setsockopt(_socketFD, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t)) != 0) {
#endif
		closesocket(_socketFD);
		return false;
	}

	if (bind(_socketFD, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
		// Handle the error.
		cleanup();
		FD_CLR(_socketFD, &_readFDSet);
		FD_CLR(_socketFD, &_writeFDSet);
		closesocket(_socketFD);
		_socketFD = INVALID_SOCKET;
		return false;
	}

	if (listen(_socketFD, 5) < 0) {
		// Handle the error.
		cleanup();
		closesocket(_socketFD);
		_socketFD = INVALID_SOCKET;
		return false;
	}

#ifdef O_NONBLOCK
	fcntl(_socketFD, F_SETFL, O_NONBLOCK);
#endif
#ifdef WIN32
	unsigned long mode = 1;
	ioctlsocket(_socketFD, FIONBIO, &mode);
#endif

	_maxFD = std::max(_socketFD + 1, _maxFD);
	FD_SET(_socketFD, &_readFDSet);

	return true;
}

Network::ClientSocketsIter Network::closeClient(ClientSocketsIter& i) {
	closeClient(*i);
	return _clientSockets.erase(i);
}

void Network::closeClient(Client& client) {
	const SOCKET clientSocket = client.socket;
	FD_CLR(clientSocket, &_readFDSet);
	FD_CLR(clientSocket, &_writeFDSet);
	closesocket(clientSocket);
	client.socket = INVALID_SOCKET;
	for (Listeners::iterator i = _listeners.begin(); i != _listeners.end(); ++i) {
		(*i)->onDisconnect(&client);
	}
}

void Network::update(long /*deltaTime*/) {
	fd_set readFDsOut;
	fd_set writeFDsOut;

	memcpy(&readFDsOut, &_readFDSet, sizeof(readFDsOut));
	memcpy(&writeFDsOut, &_writeFDSet, sizeof(writeFDsOut));

	struct timeval tv;
	const int timeout = 10;
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = 1000 * (timeout % 1000);
	const int ready = select(_maxFD, &readFDsOut, &writeFDsOut, nullptr, &tv);
	if (ready < 0) {
		return;
	}
	if (_socketFD != INVALID_SOCKET && FD_ISSET(_socketFD, &readFDsOut)) {
		const SOCKET clientSocket = accept(_socketFD, nullptr, 0);
		if (clientSocket != INVALID_SOCKET) {
			_maxFD = std::max(clientSocket + 1, _maxFD);
			FD_SET(clientSocket, &_readFDSet);
			Client c(clientSocket);
			_clientSockets.push_back(c);
			for (Listeners::iterator i = _listeners.begin(); i != _listeners.end(); ++i) {
				(*i)->onConnect(&c);
			}
		}
	}

	ClientId clientId = 0;
	for (ClientSocketsIter i = _clientSockets.begin(); i != _clientSockets.end(); ++clientId) {
		Client& client = *i;
		const SOCKET clientSocket = client.socket;
		if (clientSocket == INVALID_SOCKET) {
			i = closeClient(i);
			continue;
		}

		if (FD_ISSET(clientSocket, &writeFDsOut)) {
			if (!client.out.empty()) {
				uint8_t buf[4096];
				const std::size_t len = std::min(sizeof(buf), client.out.size());
				for (std::size_t n = 0; n < len; ++n) {
					buf[n] = client.out.at(n);
				}
				const int sent = send(clientSocket, buf, len, 0);
				if (sent < 0) {
					i = closeClient(i);
					continue;
				}
				for (int n = 0; n < sent; ++n) {
					client.out.pop_front();
				}
			} else if (client.finished) {
				i = closeClient(i);
				continue;
			}
		}

		if (FD_ISSET(clientSocket, &readFDsOut)) {
			uint8_t buf[4096];
			const int len = recv(clientSocket, buf, sizeof(buf), 0);
			if (len < 0) {
				i = closeClient(i);
				continue;
			}
			for (int n = 0; n < len; ++n) {
				client.in.push_back(buf[n]);
			}
		}

		ProtocolMessageFactory& factory = ProtocolMessageFactory::get();
		if (factory.isNewMessageAvailable(client.in)) {
			ScopedPtr<IProtocolMessage> msg(factory.create(client.in));
			if (!msg) {
				i = closeClient(i);
				continue;
			} else {
				const ProtocolHandlerPtr& handler = ProtocolHandlerRegistry::get().getHandler(*msg);
				if (handler)
					handler->execute(clientId, *msg);
			}
		}
		++i;
	}
}

void Network::broadcast(const IProtocolMessage& msg) {
	if (_clientSockets.empty())
		return;
	streamContainer out;
	msg.serialize(out);
	for (ClientSocketsIter i = _clientSockets.begin(); i != _clientSockets.end(); ++i) {
		Client& client = *i;
		if (client.socket == INVALID_SOCKET) {
			i = closeClient(i);
			continue;
		}

		IProtocolMessage::addInt(client.out, static_cast<int32_t>(out.size()));
		std::copy(out.begin(), out.end(), std::back_inserter(client.out));
		FD_SET(client.socket, &_writeFDSet);
	}
}

bool Network::sendToClient(Client* client, const IProtocolMessage& msg) {
	assert(client != nullptr);
	streamContainer out;
	msg.serialize(out);

	if (client->socket == INVALID_SOCKET) {
		closeClient(*client);
		return false;
	}

	IProtocolMessage::addInt(client->out, static_cast<int32_t>(out.size()));
	std::copy(out.begin(), out.end(), std::back_inserter(client->out));
	FD_SET(client->socket, &_writeFDSet);
	return true;
}

}
