#include "./ParticleEmitter.h"

namespace breakout {
	ParticleEmitter::ParticleEmitter(Texture* sprite, unsigned int amount)
		: m_Sprite(sprite), m_Amount(amount), m_LastUsedParticle(0) {
		for (unsigned int i = 0; i < this->m_Amount; i++) {
			this->m_Particles.push_back(Particle());
		}
	}

	void ParticleEmitter::Draw(SpriteRenderer& renderer) {
		// use additive blending to give it a glow effect
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));

		for (Particle particle : this->m_Particles) {
			renderer.DrawSprite(this->m_Sprite, particle.Position, glm::vec2(10.0f, 10.0f), 0.0f, particle.Color);
		}

		// reset to default blending mode
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void ParticleEmitter::Update(float deltaTime, GameObject& gameobject, unsigned int newParticles, glm::vec2 offset) {
		// spawn new particles
		for (unsigned int i = 0; i < newParticles; i++) {
			int unusedParticle = this->firstUnusedParticle();
			this->respawnParticle(this->m_Particles[unusedParticle], gameobject, offset);
		}

		// update all particles
		for (unsigned int i = 0; i < this->m_Amount; i++) {
			Particle& particle = this->m_Particles[i];
			particle.Life -= deltaTime;
			if (particle.IsAlive()) {
				particle.Position -= particle.Velocity * deltaTime;
				particle.Color.a -= deltaTime * 2.5f;
			}
		}
	}

	unsigned int ParticleEmitter::firstUnusedParticle() {
		// search from last used particle, will usually return almost immediately
		for (unsigned int i = this->m_LastUsedParticle; i < this->m_Amount; i++) {
			Particle& particle = this->m_Particles[i];
			if (particle.IsDead()) {
				return m_LastUsedParticle = i;
			}
		}

		// otherwise, do linear search
		for (unsigned int i = 0; i < m_LastUsedParticle; i++) {
			Particle& particle = this->m_Particles[i];
			if (particle.IsDead()) {
				return m_LastUsedParticle = i;
			}
		}

		return m_LastUsedParticle = 0;
	}

	void ParticleEmitter::respawnParticle(Particle& particle, GameObject& gameobject, glm::vec2 offset) {
		float random = ((rand() % 100) - 50) / 10.0f;
		float color = 0.5f + ((rand() % 100) / 100.0f);
		particle.Position = gameobject.GetPosition() + random + offset;
		particle.Color = glm::vec4(color, color, color, 1);
		particle.Life = 1.0f;
		particle.Velocity = gameobject.Velocity * 0.1f;
	}
}