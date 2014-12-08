#pragma once

#include "IProtocolMessage.h"

namespace ai {

class ProtocolMessageFactory {
private:
	ProtocolMessageFactory();
public:

	static ProtocolMessageFactory& get() {
		static ProtocolMessageFactory _instance;
		return _instance;
	}

	/**
	 * @brief Checks whether a new message is available in the stream
	 */
	bool isNewMessageAvailable(const streamContainer& in) const;

	/**
	 * @brief Call this only if @c isNewMessageAvailable returned @c true on the same @c streamContainer before!
	 */
	IProtocolMessage *create(streamContainer& in);
};

}
