#pragma once

#include <vector>
#include <string>
#include <algorithm>

namespace ai {

class IParser {
private:
	std::string _error;

protected:
	void setError(const std::string& error);

	inline std::string eraseAllSpaces(const std::string& str) {
		std::string tmp = str;
		tmp.erase(std::remove(tmp.begin(), tmp.end(), ' '), tmp.end());
		return tmp;
	}

	inline void splitString(const std::string& string, std::vector<std::string>& tokens, const std::string& delimiters = "()") {
		// Skip delimiters at beginning.
		std::string::size_type lastPos = string.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		std::string::size_type pos = string.find_first_of(delimiters, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos) {
			// Found a token, add it to the vector.
			tokens.push_back(string.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = string.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = string.find_first_of(delimiters, lastPos);
		}
	}

	inline std::string getBetween (const std::string& str, const std::string& tokenStart, const std::string& tokenEnd) {
		const std::size_t start = str.find(tokenStart);
		if (start == std::string::npos)
			return "";

		const std::size_t end = str.find(tokenEnd);
		if (end == std::string::npos) {
			setError("syntax error - expected " + tokenEnd);
			return "";
		}
		const size_t startIndex = start + 1;
		const size_t endIndex = end - startIndex;
		if (endIndex <= 0) {
			return "";
		}
		const std::string& between = str.substr(startIndex, endIndex);
		return between;
	}

public:
	const std::string& getError() const;
};

inline void IParser::setError(const std::string& error) {
	_error = error;
}

inline const std::string& IParser::getError() const {
	return _error;
}

}
