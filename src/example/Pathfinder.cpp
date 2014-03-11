#include "Pathfinder.h"

namespace ai {
namespace example {

Pathfinder::Pathfinder(int width, int height) :
		IPathfinder(), _width(width), _height(height) {
}

Pathfinder::~Pathfinder() {
}

ai::MoveState Pathfinder::move(ai::AI& entity, const ai::AIPosition& to, std::list<ai::AIPosition>& route) {
	if (to.x() < 0 || to.y() < 0)
		return ai::IMPOSSIBLE;
	if (to.x() >= _width || to.y() >= _height)
		return ai::IMPOSSIBLE;

	ai::ICharacter& chr = entity.getCharacter();
	ai::AIPosition pos = chr.getPosition();

	while (pos != to) {
		const float x = to.x() - pos.x() > 0.0f ? 1.0f : -1.0f;
		const float y = to.y() - pos.y() > 0.0f ? 1.0f : -1.0f;
		pos.setX(pos.x() + x);
		pos.setY(pos.y() + y);

		// plain 2d - no z
		route.push_back(pos);
	}

	return ai::SUCCESSFUL;
}

}
}
