#include <tree/Invert.h>

namespace ai {

TreeNodePtr Invert::Factory::create(const TreeNodeFactoryContext *ctx) const {
	return TreeNodePtr(new Invert(ctx->name));
}

Invert::Factory Invert::FACTORY;

}
