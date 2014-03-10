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
	static TypeMap _types;

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
		TypeMapConstIter i = _types.find(id);
		if (i != _types.end())
			return *i->second;

		return NONE;
	}

	static const T& getByName (const std::string& name)
	{
		for (TypeMapConstIter i = _types.begin(); i != _types.end(); ++i) {
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
		return _types.begin();
	}

	static inline TypeMapConstIter end ()
	{
		return _types.end();
	}
};

template <typename T>
uint32_t Enum<T>::_cnt = 0;

template <typename T>
typename Enum<T>::TypeMap Enum<T>::_types;

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
	_types.insert(std::pair<uint32_t, T*>(id, static_cast<T*>(this)));
}

}
