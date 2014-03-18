#pragma once

#include <AI.h>

namespace ai {
namespace example {

class Pathfinder: public ai::IPathfinder {
protected:
	int _width;
	int _height;

	inline const ai::AIPosition& getRandomDirection() const;
public:
	Pathfinder(int width, int height);
	virtual ~Pathfinder();

	ai::MoveState move(ai::AI& entity, const ai::AIPosition& to, std::list<ai::AIPosition>& route) override;

	// returns a random start position within the boundaries
	ai::AIPosition getStartPosition();
};

}
}
