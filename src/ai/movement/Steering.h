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

class TargetSeek: public ISteering {
protected:
	Vector3f _target;
public:
	TargetSeek(ICharacter& character, float speed, const Vector3f& target) :
			ISteering(character, speed), _target(target) {
	}

	MoveVector execute () const override {
		Vector3f v = _target;
		v -= _character.getPosition();
		if (v.squareLength() > 0) {
			v.normalize();
			v *= _speed;
		}
		const MoveVector d(v, 0.0f);
		return d;
	}
};

class GroupSeek: public TargetSeek {
public:
	GroupSeek(AI& ai, float speed, GroupId groupId) :
		TargetSeek(ai.getCharacter(), speed, ai.getGroupMgr().getPosition(groupId)) {
	}
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
