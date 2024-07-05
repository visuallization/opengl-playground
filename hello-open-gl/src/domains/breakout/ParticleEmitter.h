#pragma once

#include <vector>

#include "Shader.h"
#include "SpriteRenderer.h"
#include "Texture.h"

#include "./GameObject.h"
#include "./Particle.h"


namespace breakout {
	class ParticleEmitter {
	public:
		ParticleEmitter(Texture* sprite, unsigned int amount);

		void Update(float deltaTime, GameObject& gameobject, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f));
		void Draw(SpriteRenderer& renderer);

	private:
		std::vector<Particle> m_Particles;
		unsigned int m_Amount;
		unsigned int m_FirstUnusedParticle;

		Texture* m_Sprite;

		void respawnParticle(Particle& particle, GameObject& gameobject, glm::vec2 offset = glm::vec2(0.0f));
	};
}
