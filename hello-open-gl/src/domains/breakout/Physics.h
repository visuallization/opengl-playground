#pragma once

#include <glm/glm.hpp>

#include "./CollisionShape.h"
#include "./CollisionShapeCircle.h"
#include "./CollisionShapeRectangle.h"
#include "./GameObject.h"

namespace breakout {
	enum Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	struct Collision {
		bool IsActive;
		Direction Orientation;
		glm::vec2 Penetration;

		Collision(bool isActive, Direction orientation, glm::vec2 penetration)
			: IsActive(isActive), Orientation(orientation), Penetration(penetration) {}
	};

	class Physics {
	public:
		static bool IsColliding(const CollisionShapeRectangle& one, const CollisionShapeRectangle& two);
		static Collision IsColliding(const CollisionShapeCircle& one, const CollisionShapeCircle& two);
		static Collision IsColliding(const CollisionShapeRectangle& one, const CollisionShapeCircle& two);
		static Collision IsColliding(const CollisionShapeCircle& one, const CollisionShapeRectangle& two);

	private:
		Physics();

		static Direction getCollisionOrientation(glm::vec2 target);
	};
}