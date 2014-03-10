#pragma once

#include <list>
#include <common/AIPosition.h>
#include <common/Compiler.h>

namespace ai {

enum MoveState {
	IMPOSSIBLE, SUCCESSFUL
};

class IPathfinder {
public:
	virtual ~IPathfinder() {
	}

	virtual MoveState move(AI& entity, const AIPosition& to, std::list<AIPosition>& route) = 0;

	virtual MoveState move(AI& entity, const AIPosition& to) {
		std::list<AIPosition> route;
		return move(entity, to, route);
	}
};

class NOPPathfinder: public IPathfinder {
public:
	virtual ~NOPPathfinder() {
	}

	MoveState move(AI& /*entity*/, const AIPosition& /*to*/, std::list<AIPosition>& /*route*/) {
		return SUCCESSFUL;
	}
};

}
