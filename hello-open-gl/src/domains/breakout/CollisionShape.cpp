#include "./CollisionShape.h"

namespace breakout {
	CollisionShape::CollisionShape()
		: Position(0), Size(1) {}
	CollisionShape::CollisionShape(glm::vec2 position, glm::vec2 size)
		: Position(position), Size(size) {}
}