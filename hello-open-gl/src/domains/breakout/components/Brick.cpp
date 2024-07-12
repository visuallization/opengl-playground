#include "Brick.h"

namespace breakout {
	Brick::Brick()
		: GameObject(), IsSolid(false) {}

	Brick::Brick(glm::vec2 position, glm::vec2 size, Texture* sprite, bool isSolid, glm::vec4 color, float rotation)
		: GameObject(position, size, sprite, rotation, color), IsSolid(isSolid) {}
}