#pragma once

#include <string>

namespace ai {

class LUACondition {
private:
	std::string _name;
public:
	LUACondition(const std::string& name) :
			_name(name) {
	}

	inline const std::string& getName () const {
		return _name;
	}
};

}
