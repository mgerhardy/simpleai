#include "Idle.h"

namespace ai {

TreeNodeStatus Idle::executeTimed(AI& entity, long deltaMillis) {
	return RUNNING;
}

TreeNodePtr Idle::Factory::create(const TreeNodeFactoryContext *ctx) const {
	return TreeNodePtr(new Idle(ctx->name, ctx->parameters, ctx->condition));
}

Idle::Factory Idle::FACTORY;

}
