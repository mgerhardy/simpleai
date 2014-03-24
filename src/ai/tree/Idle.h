#pragma once

#include <tree/ITimedNode.h>

namespace ai {

class Idle: public ai::ITimedNode {
public:
	TIMERNODE_CLASS(Idle);

	TreeNodeStatus executeTimed(AI& entity, long deltaMillis) override;
};

}
