#pragma once

#include <AI.h>
#include <pathfinding/IMap.h>

namespace ai {
namespace example {

class Pathfinder: public ai::IPathfinder {
protected:
	const ai::IMap& _map;

	inline const ai::AIPosition& getRandomDirection() const;
public:
	Pathfinder(const ai::IMap& map);
	virtual ~Pathfinder();

	ai::MoveState move(ai::AI& entity, const ai::AIPosition& to, std::list<ai::AIPosition>& route) override;
};

}
}
