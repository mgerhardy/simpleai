#pragma once

#include "common/Vector3f.h"
#include "common/Thread.h"
#include <string>
#include <map>

namespace ai {

/**
 * @brief Defines some standard names for @c ICharacter attributes. None of these must be used. But if you
 * use them, the remote debugger can make use of known values to render more information in to the view.
 */
namespace attributes {
/**
 * @brief Attribute for the name of an entity
 */
const char* const NAME = "Name";
const char* const GROUP = "Group";
const char* const ID = "Id";
const char* const POSITION = "Position";
const char* const SPEED = "Speed";
const char* const ORIENTATION = "Orientation";
}

/**
 * @brief Defines the type of the id to identify an ai controlled entity.
 *
 * @note @c -1 is reserved. You should use ids >= 0
 */
typedef int CharacterId;

typedef std::map<std::string, std::string> CharacterAttributes;

/**
 * @brief Class that should be extended by the ai controlled entity class.
 *
 * It uses a @c CharacterId to identify the character in the game. The @c AI class
 * has a reference to its controlled @c ICharacter instance.
 */
class ICharacter {
protected:
	CharacterId _id;
	ATOMIC(Vector3f) _position;
	float _orientation;
	// m/s
	float _speed;
	CharacterAttributes _attributes;

public:
	ICharacter(CharacterId id) :
			_id(id), _orientation(0.0f), _speed(0.0f) {
	}

	virtual ~ICharacter() {
	}

	bool operator ==(const ICharacter& character) const;
	bool operator !=(const ICharacter& character) const;

	CharacterId getId() const;
	void setPosition(const Vector3f& position);
	Vector3f getPosition() const;
	void setOrientation(float orientation);
	/**
	 * @return the radians around the y (up) axis
	 */
	float getOrientation() const;
	void setSpeed(float speed);
	float getSpeed() const;
	void setAttribute(const std::string& key, const std::string& value);
	const CharacterAttributes& getAttributes() const;
};

inline void ICharacter::setPosition(const Vector3f& position) {
	_position = position;
}

inline void ICharacter::setOrientation (float orientation) {
	_orientation = orientation;
}

inline float ICharacter::getOrientation () const {
	return _orientation;
}

inline void ICharacter::setAttribute(const std::string& key, const std::string& value) {
	_attributes[key] = value;
}

inline const CharacterAttributes& ICharacter::getAttributes() const {
	return _attributes;
}

inline bool ICharacter::operator ==(const ICharacter& character) const {
	return character._id == _id;
}

inline bool ICharacter::operator !=(const ICharacter& character) const {
	return character._id != _id;
}

inline CharacterId ICharacter::getId() const {
	return _id;
}

inline Vector3f ICharacter::getPosition() const {
	return _position;
}

inline void ICharacter::setSpeed(float speed) {
	_speed = speed;
}

inline float ICharacter::getSpeed() const {
	return _speed;
}

template <typename CharacterType>
inline const CharacterType& character_cast(const ICharacter& character) {
	return static_cast<const CharacterType&>(character);
}

template <typename CharacterType>
inline CharacterType& character_cast(ICharacter& character) {
	return static_cast<CharacterType&>(character);
}

}
