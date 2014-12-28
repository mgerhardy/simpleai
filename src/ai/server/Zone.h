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

	const AI* find(CharacterId id) const;

	AI* find(CharacterId id);

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

inline const AI* Zone::find(CharacterId id) const {
	auto i = _ais.find(id);
	if (i == _ais.end())
		return nullptr;
	return i->second;
}

inline AI* Zone::find(CharacterId id) {
	auto i = _ais.find(id);
	if (i == _ais.end())
		return nullptr;
	return i->second;
}

}
