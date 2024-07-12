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
		ParticleEmitter(Texture* sprite, unsigned int amount, glm::vec4 color = glm::vec4(1.0f));

		void Update(float deltaTime, GameObject& gameobject, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f));
		void Draw(SpriteRenderer& renderer);

	private:
		std::vector<Particle> m_Particles;
		unsigned int m_Amount;
		unsigned int m_LastUsedParticle;
		glm::vec4 m_Color;

		Texture* m_Sprite;

		unsigned int firstUnusedParticle();
		void respawnParticle(Particle& particle, GameObject& gameobject, glm::vec2 offset = glm::vec2(0.0f));
	};
}
