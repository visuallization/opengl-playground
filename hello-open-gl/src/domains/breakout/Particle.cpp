#include "./Particle.h"

namespace breakout {
	Particle::Particle(): Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}

	Particle::Particle(glm::vec4 color) : Position(0.0f), Velocity(0.0f), Color(color), Life(0.0f) {}

	Particle::Particle(glm::vec2 position, glm::vec4 color, float life)
		: Position(position), Velocity(0.0f), Color(color), Life(life) {}

	bool Particle::IsAlive() {
		return this->Life > 0.0f;
	}

	bool Particle::IsDead() {
		return !this->IsAlive();
	}
}