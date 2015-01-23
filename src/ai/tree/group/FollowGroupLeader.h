#pragma once

#include "tree/ITask.h"
#include "common/String.h"
#include "group/GroupMgr.h"

namespace ai {

/**
 * Follow the leader of a particular group
 */
class FollowGroupLeader: public ITask {
protected:
	GroupId _groupId;
public:
	TASK_CLASS_CTOR(FollowGroupLeader) {
		if (_parameters.empty()) {
			_groupId = -1;
		} else {
			_groupId = std::stoi(_parameters);
		}
	}
	NODE_FACTORY

	TreeNodeStatus doAction(AI& entity, long deltaMillis) override;
};

}
