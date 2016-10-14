/**
 * @file
 * @ingroup LUA
 */
#pragma once

#include "common/LUA.h"

namespace ai {

static inline const char *lua_metaai() {
	return "__meta_ai";
}

static inline const char *lua_metazone() {
	return "__meta_zone";
}

static inline const char* lua_metaaggromgr() {
	return "__meta_aggromgr";
}

static inline const char* lua_metacharacter() {
	return "__meta_character";
}

template<class T>
static inline T* lua_getaiudata(lua_State* s, int n, const char *name) {
	return *(T **) luaL_checkudata(s, n, name);
}

template<class T>
static inline T* lua_ainewuserdata(lua_State* s, T* ptr) {
	T ** udata = (T**) lua_newuserdata(s, sizeof(T*));
	*udata = ptr;
	return ptr;
}

template<class T>
static inline int lua_pushaiudata(lua_State* s, T* ptr, const char *name) {
	T ** udata = (T **) lua_newuserdata(s, sizeof(T *));
	luaL_getmetatable(s, name);
	if (!lua_istable(s, -1)) {
		ai_log_error("LUA: metatable for %s doesn't exist", name);
		return 0;
	}
	lua_setmetatable(s, -2);
	*udata = ptr;
	return 1;
}

/**
 * @see @ai{LUAAIRegistry::pushAIMetatable()}
 */
static AI* lua_ctxai(lua_State * s, int n) {
	return lua_getaiudata<AI>(s, n, lua_metaai());
}

static Zone* lua_ctxzone(lua_State * s, int n) {
	return lua_getaiudata<Zone>(s, n, lua_metazone());
}

static AggroMgr* lua_ctxaggromgr(lua_State * s, int n) {
	return lua_getaiudata<AggroMgr>(s, n, lua_metaaggromgr());
}

static ICharacter* lua_ctxcharacter(lua_State * s, int n) {
	return lua_getaiudata<ICharacter>(s, n, lua_metacharacter());
}

static int lua_pushzone(lua_State* s, Zone* zone) {
	return lua_pushaiudata<Zone>(s, zone, lua_metazone());
}

static int lua_pushaggromgr(lua_State* s, AggroMgr* aggroMgr) {
	return lua_pushaiudata<AggroMgr>(s, aggroMgr, lua_metaaggromgr());
}

static int lua_pushcharacter(lua_State* s, ICharacter* character) {
	return lua_pushaiudata<ICharacter>(s, character, lua_metacharacter());
}

static int lua_pushai(lua_State* s, AI* ai) {
	return lua_pushaiudata<AI>(s, ai, lua_metaai());
}

}
