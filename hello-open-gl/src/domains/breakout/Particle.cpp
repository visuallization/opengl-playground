#include "./Particle.h"

namespace breakout {
	Particle::Particle(): Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}

	bool Particle::IsAlive() {
		return this->Life > 0.0f;
	}

	bool Particle::IsDead() {
		return !this->IsAlive();
	}
}