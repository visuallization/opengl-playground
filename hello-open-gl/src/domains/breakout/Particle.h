#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace breakout {
	struct Particle {
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 Color;
		float Life;

		Particle();
		bool IsAlive();
		bool IsDead();
	};
}
