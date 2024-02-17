#include "./CollisionShapeRectangle.h"

namespace breakout {
	CollisionShapeRectangle::CollisionShapeRectangle()
		: Position(0), Size(0) {}
	CollisionShapeRectangle::CollisionShapeRectangle(glm::vec2 position, glm::vec2 size)
		: Position(position), Size(size) {}
}