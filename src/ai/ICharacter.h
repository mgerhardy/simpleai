#pragma once

#include <common/Vector3f.h>
#include <string>
#include <map>

namespace ai {

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
	Vector3f _position;
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
	const Vector3f& getPosition() const;
	void setOrientation(float orientation);
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

inline const Vector3f& ICharacter::getPosition() const {
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
