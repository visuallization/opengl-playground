#pragma once

#include "glm/gtc/matrix_transform.hpp"


class CollisionShapeCircle {
public:
	glm::vec2 Position;
	glm::vec2 Size;
	float Radius;
};

namespace breakout {
	class CollisionShapeRectangle {
	public:
		glm::vec2 Position;
		glm::vec2 Size;

		CollisionShapeRectangle();
		CollisionShapeRectangle(glm::vec2 position, glm::vec2 size);
	};
}