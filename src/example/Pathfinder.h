#pragma once

#include <AI.h>

namespace ai {
namespace example {

class Pathfinder: public ai::IPathfinder {
protected:
	int _width;
	int _height;

	ai::AIPosition getRandomDirection(ai::ICharacter& chr);
public:
	Pathfinder(int width, int height);
	virtual ~Pathfinder();

	ai::MoveState move(ai::AI& entity, const ai::AIPosition& to, std::list<ai::AIPosition>& route) override;
};

}
}
