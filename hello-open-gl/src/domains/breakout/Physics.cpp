#include "./Physics.h"

#include <stdexcept>

namespace breakout {
	bool Physics::IsColliding(const GameObject& one, const GameObject& two) {
		return Physics::IsColliding(*one.Collider, *two.Collider);
	}

	bool Physics::IsColliding(CollisionShape& one, CollisionShape& two) {
		CollisionShapeRectangle* rectangleOne = static_cast<CollisionShapeRectangle*>(&one);
		CollisionShapeRectangle* rectangleTwo = static_cast<CollisionShapeRectangle*>(&two);
		return Physics::IsColliding(*rectangleOne, *rectangleTwo);
	}

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
		glm::vec2 center(one.Position + one.Radius);

		glm::vec2 aabbHalf(two.Size.x / 2.f, two.Size.y / 2.f);
		glm::vec2 aabbCenter(two.Position.x + aabbHalf.x, two.Position.y + aabbHalf.y);

		glm::vec2 difference = center - aabbCenter;
		glm::vec2 clamped = glm::clamp(difference, -aabbHalf, aabbHalf);

		glm::vec2 closest = aabbCenter + clamped;
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