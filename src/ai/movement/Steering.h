/**
 * @file
 * @brief Defines some basic movement algorithms like Wandering, Seeking and Fleeing.
 */
#pragma once

#include "AI.h"
#include "common/Math.h"
#include "common/Random.h"
#include "common/Compiler.h"

namespace ai {
namespace movement {

/**
 * @brief Steering interface
 */
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

/**
 * @brief Seeks a particular target
 */
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

/**
 * @brief Flees from a particular target
 */
class TargetFlee: public ISteering {
protected:
	Vector3f _target;
public:
	TargetFlee(ICharacter& character, float speed, const Vector3f& target) :
			ISteering(character, speed), _target(target) {
	}

	MoveVector execute () const override {
		Vector3f v = _character.getPosition();
		v -= _target;
		if (v.squareLength() > 0) {
			v.normalize();
			v *= _speed;
		}
		const MoveVector d(v, 0.0f);
		return d;
	}
};

/**
 * @brief Seeks a particular group
 */
class GroupSeek: public TargetSeek {
public:
	GroupSeek(AI& ai, float speed, GroupId groupId) :
		TargetSeek(ai.getCharacter(), speed, ai.getGroupMgr().getPosition(groupId)) {
	}
};

/**
 * @brief Flees from a particular group
 */
class GroupFlee: public TargetFlee {
public:
	GroupFlee(AI& ai, float speed, GroupId groupId) :
		TargetFlee(ai.getCharacter(), speed, ai.getGroupMgr().getPosition(groupId)) {
	}
};

/**
 * @brief Moves forward in the direction the character is currently facing into. Changes
 * orientation (resp. rotation) in a range of [-rotation,rotation] where more weight is
 * given to keep the current orientation.
 */
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

/**
 * @brief Steering and weight as input for @c WeightedSteering
 */
struct WeightedData {
	const ISteering *steering;
	const float weight;

	WeightedData(const ISteering *_steering, float _weight = 1.0f) :
			steering(_steering), weight(_weight) {
		assert(weight > 0.0001f);
	}
};
typedef std::vector<WeightedData> WeightedSteerings;
typedef WeightedSteerings::const_iterator WeightedSteeringsIter;

/**
 * This class allows you to weight several steering methods and get a blended @c MoveVector out of it.
 */
class WeightedSteering {
private:
	WeightedSteerings _steerings;
public:
	WeightedSteering(const WeightedSteerings& steerings) :
			_steerings(steerings) {
	}

	MoveVector execute () const {
		float totalWeight = 0.0f;
		Vector3f vecBlended;
		float angularBlended;
		for (WeightedSteeringsIter i = _steerings.begin(); i != _steerings.end(); ++i) {
			const WeightedData& wd = *i;
			const float weight = wd.weight;
			const MoveVector& mv = wd.steering->execute();
			vecBlended += mv.getVector() * weight;
			angularBlended += mv.getRotation() * weight;
			totalWeight += weight;
		}

		const float scale = 1.0f / totalWeight;
		return MoveVector(vecBlended * scale, angularBlended * scale);
	}
};

}
}
