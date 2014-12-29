#include "GameEntity.h"
#include "GameMap.h"

namespace ai {
namespace example {

void GameEntity::update(long deltaTime) {
	ICharacter::update(deltaTime);
	const float sizeF = static_cast<float>(_map->getSize());
	Vector3f newPos(0.0f, 0.0f, 0.0f);
	Vector3f currentPos = _position;
	if (currentPos.x < -sizeF) {
		newPos.x = sizeF;
	} else if (currentPos.x > sizeF) {
		newPos.x = -sizeF;
	}
	if (currentPos.z < -sizeF) {
		newPos.z = sizeF;
	} else if (currentPos.z > sizeF) {
		newPos.z = -sizeF;
	}
	setPosition(newPos);
	// TODO: switch direction when the respawn on another side of the map

	std::stringstream ss;
	ss << _position;
	setAttribute(ai::attributes::POSITION, ss.str());
	setAttribute(ai::attributes::SPEED, std::to_string(getSpeed()));
	setAttribute(ai::attributes::ORIENTATION, std::to_string(toDegrees(getOrientation())));
}

}
}
