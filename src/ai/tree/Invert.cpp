#include <tree/Invert.h>
#include <conditions/True.h>

namespace ai {

TreeNodePtr Invert::Factory::create(const TreeNodeFactoryContext *ctx) const {
	return TreeNodePtr(new Invert(ctx->name, ctx->parameters, ctx->condition));
}

Invert::Factory Invert::FACTORY;

}
