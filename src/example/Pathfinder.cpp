#include "Pathfinder.h"
#include <stdlib.h>

namespace ai {
namespace example {

Pathfinder::Pathfinder(const ai::IMap& map) :
		IPathfinder(), _map(map) {
}

Pathfinder::~Pathfinder() {
}

ai::MoveState Pathfinder::move(ai::AI& entity, const ai::AIPosition& to, std::list<ai::AIPosition>& route) {
	if (_map.isBlocked(to))
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
