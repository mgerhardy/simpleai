#pragma once

#include <AI.h>
#include "common/Thread.h"
#include "common/Types.h"
#include <unordered_map>

namespace ai {

/**
 * @brief A zone represents one logical zone that groups AI instances.
 *
 * You have to update the AI instances in this zone in your tick by calling
 * @c Zone::update.
 *
 * Zones should have unique names - otherwise the @c Server won't be able to
 * select a particular @c Zone to debug it.
 */
class Zone {
public:
	typedef std::unordered_map<CharacterId, AI*> AIMap;
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
	 * @brief Update all the @c ICharacter and @c AI instances in this zone.
	 * @param dt Delta time in millis since the last update call happened
	 * @note You have to call this on your own.
	 */
	void update(long dt);

	/**
	 * @brief call then when you spawn a new @code AI that should be traceable via the debug viewer.
	 *
	 * @note Make sure to also call @c removeAI whenever you despawn the given @c AI instance
	 * @note This locks the zone
	 */
	bool addAI(AI* ai);
	/**
	 * @note This locks the zone
	 */
	bool removeAI(const AI* ai);

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

	/**
	 * @brief Executes a lambda or functor for the given character
	 *
	 * @return @c true if the func was called for the character, @c false if not
	 * e.g. in the case the given @c CharacterId wasn't found in this zone.
	 *
	 * @note This locks the zone
	 */
	template<typename Func>
	bool execute(CharacterId id, const Func& func) const {
		SCOPEDLOCK(_mutex);
		auto i = _ais.find(id);
		if (i == _ais.end())
			return false;
		AI *ai = i->second;
		func(*ai);
		return true;
	}

	template<typename Func>
	void visit(Func& func) {
		SCOPEDLOCK(_mutex);
		for (auto i = _ais.begin(), end = _ais.end(); i != end; ++i) {
			AI *ai = i->second;
			func(*ai);
		}
	}

	template<typename Func>
	void visit(const Func& func) const {
		SCOPEDLOCK(_mutex);
		for (auto i = _ais.begin(), end = _ais.end(); i != end; ++i) {
			const AI *ai = i->second;
			func(*ai);
		}
	}
};

inline void Zone::setDebug (bool debug) {
	_debug = debug;
}

inline const std::string& Zone::getName() const {
	return _name;
}

}
