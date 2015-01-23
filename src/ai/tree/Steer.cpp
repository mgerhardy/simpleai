#include "Steer.h"
#include "common/Random.h"
#include "movement/Steering.h"
#include <iostream>

namespace ai {

TreeNodeStatus Steer::doAction(AI& entity, long deltaMillis) {
	ICharacter& chr = entity.getCharacter();
	const movement::WeightedSteering w(_weightedSteerings);
	const MoveVector& mv = w.execute(chr, chr.getSpeed());
	if (mv.getVector().isInfinite())
		return FAILED;

	const float deltaSeconds = static_cast<float>(deltaMillis) / 1000.0f;
	chr.setPosition(chr.getPosition() + (mv.getVector() * deltaSeconds));
	chr.setOrientation(fmodf(chr.getOrientation() + mv.getRotation() * deltaSeconds, M_2PI));
	return FINISHED;
}

TreeNodePtr Steer::Factory::create(const SteerNodeFactoryContext *ctx) const {
	return TreeNodePtr(new Steer(ctx->name, ctx->parameters, ctx->condition, ctx->steerings));
}

Steer::Factory Steer::FACTORY;

}
