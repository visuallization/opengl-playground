#include "./ParticleEmitter.h"

namespace breakout {
	ParticleEmitter::ParticleEmitter(Texture* sprite, unsigned int amount)
		: m_Sprite(sprite), m_Amount(amount) {
		for (unsigned int i = 0; i < this->m_Amount; i++) {
			this->m_Particles.push_back(Particle());
		}
	}

	void ParticleEmitter::Draw(SpriteRenderer& renderer) {
		for (Particle particle : this->m_Particles) {
			renderer.DrawSprite(this->m_Sprite, particle.Position, glm::vec2(10.0f, 10.0f), 0.0f, particle.Color);
		}
	}

	void ParticleEmitter::Update(float deltaTime, GameObject& gameobject, unsigned int newParticles, glm::vec2 offset) {
		for (unsigned int i = 0; i < this->m_Amount; i++) {
			this->respawnParticle(this->m_Particles[i], gameobject, offset);
		}
	}

	void ParticleEmitter::respawnParticle(Particle& particle, GameObject& gameobject, glm::vec2 offset) {
		float random = ((rand() % 100) - 50) / 10.0f;
		float color = 0.5f + ((rand() % 100) / 100.0f);
		particle.Position = gameobject.GetPosition() + random + offset;
		particle.Color = glm::vec3(color, color, color);
		particle.Life = 1.0f;
		particle.Velocity = gameobject.Velocity * 0.1f;
	}
}