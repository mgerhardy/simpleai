#pragma once

#include "Network.h"
#include <map>
#include "IProtocolHandler.h"

namespace ai {

class ProtocolHandlerRegistry {
private:
	typedef std::map<uint32_t, IProtocolHandler*> ProtocolHandlers;
	ProtocolHandlers _registry;

	ProtocolHandlerRegistry() {
	}

public:
	static ProtocolHandlerRegistry& get() {
		static ProtocolHandlerRegistry _instance;
		return _instance;
	}

	virtual ~ProtocolHandlerRegistry() {
		for (ProtocolHandlers::iterator i = _registry.begin(); i != _registry.end(); ++i) {
			delete i->second;
		}
	}

	inline void registerHandler(const ProtocolId& type, IProtocolHandler* handler) {
		_registry.insert(std::make_pair(type, handler));
	}

	inline IProtocolHandler* getHandler(const IProtocolMessage& msg) {
		ProtocolHandlers::iterator i = _registry.find(msg.getId());
		if (i != _registry.end())
			return i->second;

		return nullptr;
	}
};

}
