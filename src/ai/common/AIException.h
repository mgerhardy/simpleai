#pragma once

#include <string>

namespace ai {

class AIException {
protected:
	const std::string _reason;
public:
	AIException(const std::string& reason) :
			_reason(reason) {
	}
	virtual ~AIException() {
	}

	const std::string& getMessage() const;
};

inline const std::string& AIException::getMessage() const {
	return _reason;
}

}
