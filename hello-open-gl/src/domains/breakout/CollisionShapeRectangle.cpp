#include "./CollisionShapeRectangle.h"

namespace breakout {
	CollisionShapeRectangle::CollisionShapeRectangle()
		: CollisionShape() {}
	CollisionShapeRectangle::CollisionShapeRectangle(glm::vec2 position, glm::vec2 size)
		: CollisionShape(position, size) {}
}