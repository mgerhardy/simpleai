#include "Steering.h"

namespace ai {
namespace movement {

STEERING_FACTORY_IMPL(TargetSeek);
STEERING_FACTORY_IMPL(TargetFlee);
STEERING_FACTORY_IMPL(GroupSeek);
STEERING_FACTORY_IMPL(GroupFlee);
STEERING_FACTORY_IMPL(Wander);
STEERING_FACTORY_IMPL(FollowGroupLeader);

}
}
