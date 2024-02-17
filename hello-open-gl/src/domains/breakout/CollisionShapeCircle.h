#pragma once

#include "glm/gtc/matrix_transform.hpp"

namespace breakout {
	class CollisionShapeCircle {
	public:
		glm::vec2 Position;
		glm::vec2 Size;
		float Radius;

		CollisionShapeCircle();
		CollisionShapeCircle(glm::vec2 position, glm::vec2 size, float radius);
	};
}