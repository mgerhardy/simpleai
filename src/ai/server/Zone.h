#pragma once

#include <AI.h>
#include <common/Thread.h>
#include <map>

namespace ai {

/**
 * @brief A zone represents one logical zone that groups AI instances.
 *
 * You have to update the AI instances in this zone in your tick.
 */
class Zone {
public:
	typedef std::map<CharacterId, AI*> AIMap;
	typedef AIMap::const_iterator AIMapConstIter;
	typedef AIMap::iterator AIMapIter;

private:
	const std::string _name;
	AIMap _ais;
	bool _debug;
	MUTEX(_mutex);

public:
	Zone(const std::string& name) :
			_name(name), _debug(false) {
	}

	virtual ~Zone() {}

	/**
	 * @brief call then when you spawn a new @code AI that should be traceable via the debug viewer.
	 *
	 * @note Make sure to also call @c removeAI whenever you despawn the given @c AI instance
	 */
	bool addAI(AI* ai);
	bool removeAI(AI* ai);

#ifndef AI_NO_THREADING
	inline operator MUTEXTYPE&() { return _mutex; }
#endif

	/**
	 * @brief Every zone has its own name that identifies it
	 */
	const std::string& getName() const;

	/**
	 * @brief Activate the debugging for this particular server instance
	 * @param[in] debug @c true if you want to activate the debugging and send
	 * the npc states of this server to all connected clients, or @c false if
	 * none of the managed entities is broadcasted.
	 */
	void setDebug(bool debug);

	AIMapIter find(CharacterId id);
	AIMapConstIter find(CharacterId id) const;

	AIMapIter begin();
	AIMapConstIter begin() const;

	AIMapIter end();
	AIMapConstIter end() const;
};

inline void Zone::setDebug (bool debug) {
	_debug = debug;
}

inline const std::string& Zone::getName() const {
	return _name;
}

inline Zone::AIMapIter Zone::begin() {
	return _ais.begin();
}

inline Zone::AIMapConstIter Zone::begin() const {
	return _ais.begin();
}

inline Zone::AIMapIter Zone::end() {
	return _ais.end();
}

inline Zone::AIMapConstIter Zone::end() const {
	return _ais.end();
}

inline Zone::AIMapIter Zone::find(CharacterId id) {
	return _ais.find(id);
}

inline Zone::AIMapConstIter Zone::find(CharacterId id) const {
	return _ais.find(id);
}

}
