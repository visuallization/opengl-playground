#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace breakout {
	class CollisionShape {
	public:
		glm::vec2 Position;
		glm::vec2 Size;

		CollisionShape();
		CollisionShape(glm::vec2 position, glm::vec2 size);

		virtual ~CollisionShape();
	};
}