#pragma once

#include <SimpleAI.h>

namespace ai {
namespace example {

class Pathfinder: public ai::IPathfinder {
protected:
	const ai::IMap& _map;

	inline const ai::Vector3f& getRandomDirection() const;
public:
	Pathfinder(const ai::IMap& map);
	virtual ~Pathfinder();

	ai::MoveState move(ai::AI& entity, const ai::Vector3f& to, std::list<ai::MoveVector>& route) override;
};

}
}
