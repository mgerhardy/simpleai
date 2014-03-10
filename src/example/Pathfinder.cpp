#include "Pathfinder.h"

namespace ai {
namespace example {

Pathfinder::Pathfinder(int width, int height) :
		IPathfinder(), _width(width), _height(height) {
}

Pathfinder::~Pathfinder() {
}

ai::MoveState Pathfinder::move(ai::AI& entity, const ai::AIPosition& to, std::list<ai::AIPosition>& route) {
	ai::ICharacter& chr = entity.getCharacter();
	ai::AIPosition pos = getRandomDirection(chr);
	pos += chr.getPosition();
	route.push_back(pos);
	return ai::SUCCESSFUL;
}

ai::AIPosition Pathfinder::getRandomDirection(ai::ICharacter& chr) {
	const ai::AIPosition& pos = chr.getPosition();
	ai::AIPosition move(3, 3);
	if (pos.x() + move.x() < 0)
		move.setX(0);
	else if (pos.x() + move.x() >= _width)
		move.setX(0);
	if (pos.y() + move.y() < 0)
		move.setY(0);
	else if (pos.y() + move.y() >= _height)
		move.setY(0);
	return move;
}

}
}
