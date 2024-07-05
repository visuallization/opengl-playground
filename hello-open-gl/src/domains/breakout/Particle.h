#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace breakout {
	struct Particle {
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec3 Color;
		float Life;

		Particle(): Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
	};
}
