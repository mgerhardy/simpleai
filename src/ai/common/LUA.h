/**
 * @file
 * @addtogroup LUA
 * @{
 */
#pragma once

#include "common/Types.h"
#include "common/NonCopyable.h"
// TODO: redefine these
//lua_writestring
//lua_writeline
//lua_writestringerror
#include <lua.hpp>
#include <string>
#include <map>
#include <assert.h>
#include <stdlib.h>
#include <sstream>
#include <memory>

namespace ai {

namespace lua {

namespace {
const std::string META_PREFIX = "META_";

int panicCB(lua_State *L) {
	ai_log_error("Lua panic. Error message: %s", (lua_isnil(L, -1) ? "" : lua_tostring(L, -1)));
	return 0;
}

}

class LUAType {
private:
	lua_State* _state;
public:
	LUAType(lua_State* state, const std::string& name) :
			_state(state) {
		const std::string metaTable = META_PREFIX + name;
		luaL_newmetatable(_state, metaTable.c_str());
		lua_pushvalue(_state, -1);
		lua_setfield(_state, -2, "__index");
	}

	// only non-capturing lambdas can be converted to function pointers
	template<class FUNC>
	void addFunction(const std::string& name, FUNC&& func) {
		lua_pushcfunction(_state, func);
		lua_setfield(_state, -2, name.c_str());
	}

	void addValue(const char* name, const char* value) {
		lua_pushstring(_state, value);
		lua_setfield(_state, -2, name);
	}
};

/**
 * @par
 * You can find the reference lua manual at http://www.lua.org/manual/5.3/
 *
 * @par -1 and -2 are pseudo indexes, they count backwards:
 * @li -1 is top
 * @li 1 is bottom
 * @li -2 is under the top
 */
class LUA : public NonCopyable {
private:
	lua_State *_state;
	std::string _error;
	bool _destroy;

public:
	explicit LUA(lua_State *state) :
			_state(state), _destroy(false) {
	}

	explicit LUA(bool debug = false) :
			_state(luaL_newstate()), _destroy(true) {
		luaL_openlibs(_state);
		lua_atpanic(_state, panicCB);
		lua_gc(_state, LUA_GCSTOP, 0);
	}

	~LUA () {
		if (_destroy) {
			//const int bytes = lua_gc(_state, LUA_GCCOUNT, 0) * 1024 + lua_gc(_state, LUA_GCCOUNTB, 0);
			lua_gc(_state, LUA_GCRESTART, 0);
			lua_close(_state);
		}
		_state = nullptr;
	}

	inline lua_State* getState () const {
		return _state;
	}

	template<class T>
	inline T* newGlobalData (const std::string& prefix, T *userData) const {
		lua_pushlightuserdata(_state, userData);
		lua_setglobal(_state, prefix.c_str());
		return userData;
	}

	template<class T>
	static T* getGlobalData(lua_State *L, const std::string& prefix) {
		lua_getglobal(L, prefix.c_str());
		T* data = (T*) lua_touserdata(L, -1);
		lua_pop(L, 1);
		return data;
	}

	template<class T>
	static T* newUserdata(lua_State *L, const std::string& prefix, T* data) {
		T ** udata = (T **) lua_newuserdata(L, sizeof(T *));
		const std::string name = META_PREFIX + prefix;
		luaL_getmetatable(L, name.c_str());
		lua_setmetatable(L, -2);
		*udata = data;
		return data;
	}

	template<class T>
	static T* getUserData (lua_State *L, int n, const std::string& prefix) {
		const std::string name = META_PREFIX + prefix;
		return *(T **) luaL_checkudata(L, n, name.c_str());
	}

	static int returnError (lua_State *L, const std::string& error) {
		ai_log_error("LUA error: '%s'", error.c_str());
		return luaL_error(L, "%s", error.c_str());
	}

	void reg (const std::string& prefix, luaL_Reg* funcs) {
		const std::string metaTableName = META_PREFIX + prefix;
		luaL_newmetatable(_state, metaTableName.c_str());
		luaL_setfuncs(_state, funcs, 0);
		lua_pushvalue(_state, -1);
		lua_setfield(_state, -1, "__index");
		lua_setglobal(_state, prefix.c_str());
	}

	LUAType registerType (const std::string& name) {
		return LUAType(_state, name);
	}

	void setError (const std::string& error) {
		_error = error;
	}

	const std::string& error () const {
		return _error;
	}

	bool load (const std::string &luaString) {
		return load(luaString.c_str(), luaString.length());
	}

	bool load (const char *luaString, size_t len) {
		if (luaL_loadbufferx(_state, luaString, len, "", nullptr) || lua_pcall(_state, 0, LUA_MULTRET, 0)) {
			setError(lua_tostring(_state, -1));
			lua_pop(_state, 1);
			return false;
		}

		return true;
	}

	/**
	 * @param[in] function function to be called
	 */
	bool execute (const std::string &function, int returnValues = 0) {
		lua_getglobal(_state, function.c_str());
		const int ret = lua_pcall(_state, 0, returnValues, 0);
		if (ret != 0) {
			setError(lua_tostring(_state, -1));
			return false;
		}

		return true;
	}
};

}

}

/**
 * @}
 */
