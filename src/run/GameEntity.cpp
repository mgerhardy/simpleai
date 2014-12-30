#include "GameEntity.h"
#include "GameMap.h"

namespace ai {
namespace example {

void GameEntity::update(long deltaTime) {
	ICharacter::update(deltaTime);

	// cap position to the map
	const float sizeF = static_cast<float>(_map->getSize());
	Vector3f newPos;
	const Vector3f& currentPos = _position;
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

	// update attributes for debugging
	setAttribute(ai::attributes::POSITION, std::to_string(getPosition()));
	setAttribute(ai::attributes::SPEED, std::to_string(getSpeed()));
	setAttribute(ai::attributes::ORIENTATION, std::to_string(toDegrees(getOrientation())));
}

}
}
