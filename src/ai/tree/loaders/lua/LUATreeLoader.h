#pragma once

#include "tree/loaders/ITreeLoader.h"

class LUA;

namespace ai {

class LUATreeLoader: public ai::ITreeLoader {
private:
	LUA *_lua;
public:
	LUATreeLoader(const IAIFactory& aiFactory);
	bool init(const std::string& luaString);
	virtual ~LUATreeLoader();
};

}
