#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace breakout {
	class Particle {
	public:
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 Color;
		float Life;

		Particle();
		Particle(glm::vec4 color);
		Particle(glm::vec2 position, glm::vec4 color, float life = 1.0f);

		bool IsAlive();
		bool IsDead();
	};
}
