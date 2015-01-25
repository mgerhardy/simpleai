/**
 * @file
 * @brief Defines some basic movement algorithms like Wandering, Seeking and Fleeing.
 */
#pragma once

#include "AI.h"
#include "common/Math.h"
#include "common/Random.h"
#include "common/MoveVector.h"
#include "common/String.h"
#include "common/MemoryAllocator.h"
#include "ICharacter.h"
#include <cassert>

namespace ai {
namespace movement {

#define STEERING_FACTORY \
public: \
	class Factory: public ISteeringFactory { \
	public: \
		SteeringPtr create (const SteeringFactoryContext *ctx) const; \
	}; \
	static Factory FACTORY;

#define STEERING_FACTORY_SINGLETON \
public: \
	class Factory: public ISteeringFactory { \
		SteeringPtr create (const SteeringFactoryContext */*ctx*/) const { \
			return get(); \
		} \
	}; \
	static Factory FACTORY;

#define STEERING_FACTORY_IMPL(SteeringName) \
	SteeringPtr SteeringName::Factory::create(const SteeringFactoryContext *ctx) const { \
		SteeringName* c = new SteeringName(ctx->parameters); \
		return SteeringPtr(c); \
	} \
	SteeringName::Factory SteeringName::FACTORY;


/**
 * @brief Steering interface
 */
class ISteering : public MemObject {
public:
	virtual ~ISteering() {}
	virtual MoveVector execute (const ICharacter& character, float speed) const = 0;
};

/**
 * @brief Seeks a particular target
 */
class TargetSeek: public ISteering {
protected:
	Vector3f _target;
public:
	STEERING_FACTORY

	TargetSeek(const std::string& parameters) :
			ISteering() {
		_target = Vector3f::parse(parameters);
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
	Vector3f _target;
public:
	STEERING_FACTORY

	TargetFlee(const std::string& parameters) :
			ISteering() {
		_target = Vector3f::parse(parameters);
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
	GroupId _groupId;
public:
	STEERING_FACTORY

	GroupSeek(const std::string& parameters) :
			ISteering() {
		_groupId = ::atoi(parameters.c_str());
	}

	inline bool isValid () const {
		return _groupId != -1;
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		const AI& ai = character.getAI();
		const Vector3f& target = ai.getGroupPosition(_groupId);
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
	GroupId _groupId;
public:
	STEERING_FACTORY

	GroupFlee(const std::string& parameters) :
			ISteering() {
		_groupId = ::atoi(parameters.c_str());
	}

	inline bool isValid () const {
		return _groupId != -1;
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		const AI& ai = character.getAI();
		const Vector3f& target = ai.getGroupPosition(_groupId);
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
 * @brief Flees from a particular group
 */
class FollowGroupLeader: public ISteering {
protected:
	GroupId _groupId;
public:
	STEERING_FACTORY

	FollowGroupLeader(const std::string& parameters) :
			ISteering() {
		_groupId = ::atoi(parameters.c_str());
	}

	inline bool isValid () const {
		return _groupId != -1;
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		const AI& ai = character.getAI();
		const Vector3f& target = ai.getGroupLeaderPosition(_groupId);
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
	const float _rotation;
public:
	STEERING_FACTORY

	Wander(const std::string& parameter) :
			ISteering(), _rotation(parameter.empty() ? ai::toRadians(10.0f) : Str::strToFloat(parameter)) {
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
	SteeringPtr steering;
	const float weight;

	WeightedData(const SteeringPtr& _steering, float _weight = 1.0f) :
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
