#pragma once

#include "./CollisionShape.h"
#include "./CollisionShapeCircle.h"
#include "./CollisionShapeRectangle.h"
#include "./GameObject.h"

namespace breakout {
	class Physics {
	public:
		static bool IsColliding(const GameObject& one, const GameObject& two);
		static bool IsColliding(CollisionShape& one, CollisionShape& two);
		static bool IsColliding(const CollisionShapeRectangle& one, const CollisionShapeRectangle& two);
		static bool IsColliding(const CollisionShapeCircle& one, const CollisionShapeCircle& two);
		static bool IsColliding(const CollisionShapeRectangle& one, const CollisionShapeCircle& two);
		static bool IsColliding(const CollisionShapeCircle& one, const CollisionShapeRectangle& two);

	private:
		Physics();
	};
}