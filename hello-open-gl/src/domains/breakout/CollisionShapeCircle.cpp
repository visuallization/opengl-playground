#include "./CollisionShapeCircle.h"

namespace breakout {
	CollisionShapeCircle::CollisionShapeCircle()
		: CollisionShape(), Radius(0.5f) {}
	CollisionShapeCircle::CollisionShapeCircle(glm::vec2 position, glm::vec2 size, float radius)
		: CollisionShape(position, size), Radius(radius) {}

	glm::vec2 CollisionShapeCircle::GetCenter() const {
		return glm::vec2(Position + Radius);
	}

	void CollisionShapeCircle::Draw(SpriteRenderer& renderer) {
		renderer.DrawRectangle(Position, Size);
	}
}