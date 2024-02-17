#include "./CollisionShapeCircle.h"

namespace breakout {

	CollisionShapeCircle::CollisionShapeCircle()
		: Position(0), Size(0), Radius(0) {}
	CollisionShapeCircle::CollisionShapeCircle(glm::vec2 position, glm::vec2 size, float radius)
		: Position(position), Size(size), Radius(radius) {}
}