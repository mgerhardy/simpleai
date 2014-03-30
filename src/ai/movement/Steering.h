#pragma once

#include <common/Math.h>
#include <common/Compiler.h>

namespace ai {
namespace movement {

struct SteeringData {
	Vector3f linear;
	float angular;
};

class ISteering {
protected:
	ICharacter& _character;
	float _speed;
public:
	ISteering(ICharacter& character, float speed) :
			_character(character), _speed(speed) {
	}
	virtual ~ISteering() {}
	virtual SteeringData execute () const = 0;
};

class Wander: public ISteering {
protected:
	float _rotation;
public:
	Wander(ICharacter& character, float speed, float rotation) :
			ISteering(character, speed), _rotation(rotation) {
	}

	SteeringData execute () const override {
		SteeringData d;

		d.linear = Vector3f::fromRadians(_character.getOrientation());
		d.linear *= _speed;

		const float change = ai::randomBinomial();
		d.angular = change * _rotation;

		return d;
	}
};

}
}
