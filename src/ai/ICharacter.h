#pragma once

#include <common/AIPosition.h>
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
	AIPosition _position;
	CharacterAttributes _attributes;

public:
	ICharacter(CharacterId id) :
			_id(id) {
	}

	virtual ~ICharacter() {
	}

	bool operator ==(const ICharacter& character) const;
	bool operator !=(const ICharacter& character) const;

	CharacterId getId() const;
	void setPosition(const AIPosition& position);
	const AIPosition& getPosition() const;
	void setAttribute(const std::string& key, const std::string& value);
	const CharacterAttributes& getAttributes() const;
};

inline void ICharacter::setPosition(const AIPosition& position) {
	_position = position;
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

inline const AIPosition& ICharacter::getPosition() const {
	return _position;
}

}
