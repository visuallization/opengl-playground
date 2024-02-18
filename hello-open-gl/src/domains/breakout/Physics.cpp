#include "./Physics.h"

#include <stdexcept>

namespace breakout {
	bool Physics::IsColliding(const CollisionShapeRectangle& one, const CollisionShapeRectangle& two) {
		bool collisonX = one.Position.x + one.Size.x >= two.Position.x && two.Position.x + two.Size.x >= one.Position.x;
		if (!collisonX) {
			return false;
		}

		bool collisionY = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;
		if (!collisionY) {
			return false;
		}

		return true;
	}

	bool Physics::IsColliding(const CollisionShapeCircle& one, const CollisionShapeRectangle& two) {
		glm::vec2 center = one.GetCenter();

		glm::vec2 AABBHalf = two.GetAABBHalf();
		glm::vec2 AABBCenter = two.GetAABBCenter();

		glm::vec2 difference = center - AABBCenter;
		glm::vec2 clamped = glm::clamp(difference, -AABBHalf, AABBHalf);

		glm::vec2 closest = AABBCenter + clamped;
		difference = closest - center;

		return glm::length(difference) < one.Radius;
	}

	bool Physics::IsColliding(const CollisionShapeRectangle& one, const CollisionShapeCircle& two) {
		return Physics::IsColliding(two, one);
	}

	// TODO: Implement
	bool Physics::IsColliding(const CollisionShapeCircle& one, const CollisionShapeCircle& two) {
		throw std::runtime_error("This collision check has not been implemented yet!");
	}

}