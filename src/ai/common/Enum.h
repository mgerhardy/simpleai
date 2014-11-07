#pragma once

#include <stdint.h>
#include <string>
#include <map>

namespace ai {

template<typename T>
class Enum {
public:
	typedef std::map<uint32_t, T*> TypeMap;
	typedef typename TypeMap::const_iterator TypeMapConstIter;
private:
	Enum (const Enum&);
	Enum& operator= (const Enum&);
protected:
	static uint32_t _cnt;
	static inline TypeMap& getMap() {
		static TypeMap _types;
		return _types;
	}

	explicit Enum (const std::string& _name);

public:
	static T NONE;
	const uint32_t id;
	const std::string name;

	inline bool isNone() const
	{
		return *this == NONE;
	}

	static const T& get (uint32_t id)
	{
		TypeMapConstIter i = getMap().find(id);
		if (i != getMap().end())
			return *i->second;

		return NONE;
	}

	static const T& getByName (const std::string& name)
	{
		for (TypeMapConstIter i = getMap().begin(); i != getMap().end(); ++i) {
			if ((*i->second).name == name)
				return *i->second;
		}

		return NONE;
	}

	inline bool operator< (const T& other) const
	{
		return id < other.id;
	}

	inline bool operator== (const T& other) const
	{
		return id == other.id;
	}

	inline operator bool () const
	{
		return !isNone();
	}

	inline bool operator!= (const T& other) const
	{
		return !(*this == other);
	}

	static inline TypeMapConstIter begin ()
	{
		return getMap().begin();
	}

	static inline TypeMapConstIter end ()
	{
		return getMap().end();
	}
};

template <typename T>
uint32_t Enum<T>::_cnt = 0;

template <typename T>
T Enum<T>::NONE("");

template <typename T>
inline bool operator== (const Enum<T>& lhs, const Enum<T>& rhs)
{
	return lhs.id == rhs.id;
}

template <typename T>
Enum<T>::Enum (const std::string& _name) :
		id(_cnt++), name(_name)
{
	getMap().insert(std::pair<uint32_t, T*>(id, static_cast<T*>(this)));
}
