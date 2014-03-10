#pragma once

#include <common/AIPosition.h>

namespace ai {

typedef int CharacterId;

class ICharacter {
protected:
	CharacterId _id;
	AIPosition _position;

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
	AIPosition& getPosition();
};

inline void ICharacter::setPosition(const AIPosition& position) {
	_position = position;
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

inline AIPosition& ICharacter::getPosition() {
	return _position;
}

}
