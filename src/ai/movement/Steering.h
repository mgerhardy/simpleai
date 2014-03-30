#pragma once

#include <common/Math.h>
#include <common/Compiler.h>

namespace ai {
namespace movement {

class ISteering {
protected:
	ICharacter& _character;
	float _speed;
public:
	ISteering(ICharacter& character, float speed) :
			_character(character), _speed(speed) {
	}
	virtual ~ISteering() {}
	virtual MoveVector execute () const = 0;
};

class Wander: public ISteering {
protected:
	float _rotation;
public:
	Wander(ICharacter& character, float speed, float rotation) :
			ISteering(character, speed), _rotation(rotation) {
	}

	MoveVector execute () const override {
		const Vector3f& v = Vector3f::fromRadians(_character.getOrientation()) * _speed;
		const MoveVector d(v, ai::randomBinomial() * _rotation);
		return d;
	}
};

}
}
