#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

namespace ai {
namespace Str {

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

inline int toInt(const std::string& str) {
	return ::atoi(str.c_str());
}

inline float toFloat(const std::string& str) {
	return ::atof(str.c_str());
}

template <typename T>
inline std::string toString(T input) {
	std::stringstream ss;
	ss << input;
	return ss.str();
}

}
}
