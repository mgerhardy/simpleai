/**
 * @file
 * @brief Defines some basic movement algorithms like Wandering, Seeking and Fleeing.
 */
#pragma once

#include "AI.h"
#include "common/Math.h"
#include "common/Random.h"
#include "common/MoveVector.h"
#include "ICharacter.h"
#include <cassert>

namespace ai {
namespace movement {

/**
 * @brief Steering interface
 */
class ISteering {
public:
	virtual ~ISteering() {}
	virtual MoveVector execute (const ICharacter& character, float speed) const = 0;
};

/**
 * @brief Seeks a particular target
 */
class TargetSeek: public ISteering {
protected:
	const Vector3f _target;
public:
	TargetSeek(const Vector3f& target) :
			ISteering(), _target(target) {
	}

	inline bool isValid () const {
		return !_target.isInfinite();
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		if (_target.isInfinite())
			return MoveVector(_target, 0.0f);
		Vector3f v = _target - character.getPosition();
		if (v.squareLength() > 0) {
			v.normalize();
			v *= speed;
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
	const Vector3f _target;
public:
	TargetFlee(const Vector3f& target) :
			ISteering(), _target(target) {
	}

	inline bool isValid () const {
		return !_target.isInfinite();
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		if (_target.isInfinite())
			return MoveVector(_target, 0.0f);
		Vector3f v = character.getPosition();
		v -= _target;
		if (v.squareLength() > 0) {
			v.normalize();
			v *= speed;
		}
		const MoveVector d(v, 0.0f);
		return d;
	}
};

/**
 * @brief Seeks a particular group
 */
class GroupSeek: public ISteering {
protected:
	const GroupId _groupId;
	bool _leader;
public:
	GroupSeek(GroupId groupId, bool leader) :
		ISteering(), _groupId(groupId), _leader(leader) {
	}

	inline bool isValid () const {
		return _groupId != -1;
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		const AI& ai = character.getAI();
		const Vector3f& target = _leader ? ai.getGroupLeaderPosition(_groupId) : ai.getGroupPosition(_groupId);
		if (target.isInfinite())
			return MoveVector(target, 0.0f);
		Vector3f v = target - character.getPosition();
		if (v.squareLength() > 0) {
			v.normalize();
			v *= speed;
		}
		const MoveVector d(v, 0.0f);
		return d;
	}
};

/**
 * @brief Flees from a particular group
 */
class GroupFlee: public ISteering {
protected:
	const GroupId _groupId;
	bool _leader;
public:
	GroupFlee(GroupId groupId, bool leader) :
			ISteering(), _groupId(groupId), _leader(leader) {
	}

	inline bool isValid () const {
		return _groupId != -1;
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		const AI& ai = character.getAI();
		const Vector3f& target = _leader ? ai.getGroupLeaderPosition(_groupId) : ai.getGroupPosition(_groupId);
		if (target.isInfinite())
			return MoveVector(target, 0.0f);
		Vector3f v = character.getPosition() - target;
		if (v.squareLength() > 0) {
			v.normalize();
			v *= speed;
		}
		const MoveVector d(v, 0.0f);
		return d;
	}
};

/**
 * @brief Moves forward in the direction the character is currently facing into.
 *
 * Changes orientation (resp. rotation) in a range of [-rotation,rotation] where more
 * weight is given to keep the current orientation.
 */
class Wander: public ISteering {
protected:
	float _rotation;
public:
	Wander(float rotation) :
			ISteering(), _rotation(rotation) {
	}

	MoveVector execute (const ICharacter& character, float speed) const override {
		const Vector3f& v = Vector3f::fromRadians(character.getOrientation()) * speed;
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
 * @brief This class allows you to weight several steering methods and get a blended @c MoveVector out of it.
 */
class WeightedSteering {
private:
	WeightedSteerings _steerings;
public:
	WeightedSteering(const WeightedSteerings& steerings) :
			_steerings(steerings) {
	}

	MoveVector execute (const ICharacter& character, float speed) const {
		float totalWeight = 0.0f;
		Vector3f vecBlended;
		float angularBlended = 0.0f;
		for (WeightedSteeringsIter i = _steerings.begin(); i != _steerings.end(); ++i) {
			const WeightedData& wd = *i;
			const float weight = wd.weight;
			const MoveVector& mv = wd.steering->execute(character, speed);
			if (mv.getVector().isInfinite())
				continue;

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
