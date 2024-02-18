#include "./CollisionShapeRectangle.h"

namespace breakout {
	CollisionShapeRectangle::CollisionShapeRectangle()
		: CollisionShape() {}
	CollisionShapeRectangle::CollisionShapeRectangle(glm::vec2 position, glm::vec2 size)
		: CollisionShape(position, size) {}

	glm::vec2 CollisionShapeRectangle::GetAABBHalf() const {
		return glm::vec2(Size.x / 2.f, Size.y / 2.f);
	}

	glm::vec2 CollisionShapeRectangle::GetAABBCenter() const {
		glm::vec2 AABBHalf = GetAABBHalf();
		return glm::vec2(Position.x + AABBHalf.x, Position.y + AABBHalf.y);
	}
}