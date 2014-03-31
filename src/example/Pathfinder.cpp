#include "Pathfinder.h"
#include <stdlib.h>

namespace ai {
namespace example {

Pathfinder::Pathfinder(const ai::IMap& map) :
		IPathfinder(), _map(map) {
}

Pathfinder::~Pathfinder() {
}

ai::MoveState Pathfinder::move(ai::AI& /*entity*/, const ai::Vector3f& to, std::list<ai::MoveVector>& /*route*/) {
	if (_map.isBlocked(to))
		return ai::IMPOSSIBLE;

#if 0
	ai::ICharacter& chr = entity.getCharacter();
	ai::MoveVector mv(ai::Vector3f(), chr.getDirection());
	const ai::Vector3f& pos = chr.getPosition();

	while (pos != to) {
		const float x = to.x() - pos.x() > 0.0f ? 1.0f : -1.0f;
		const float z = to.z() - pos.z() > 0.0f ? 1.0f : -1.0f;
		pos.setX(pos.x() + x);
		pos.setY(pos.z() + z);

		// plain 2d - no y
		route.push_back(pos);
	}
#endif

	return ai::SUCCESSFUL;
}

}
}
