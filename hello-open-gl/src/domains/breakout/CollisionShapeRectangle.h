#pragma once

#include "./CollisionShape.h"

namespace breakout {
	class CollisionShapeRectangle: public CollisionShape {
	public:
		CollisionShapeRectangle();
		CollisionShapeRectangle(glm::vec2 position, glm::vec2 size);

		glm::vec2 GetAABBHalf() const;
		glm::vec2 GetAABBCenter() const;
	};
}