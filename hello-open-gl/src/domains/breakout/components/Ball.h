#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"

#include "../GameObject.h"

namespace breakout {
	class Ball : public GameObject {
	public:
		float Radius;
		bool IsStuck;

		Ball();
		Ball(glm::vec2 position, float radius, glm::vec2 velocity, Texture* sprite);

		glm::vec2 Move(float deltaTime, unsigned int windowWidth, unsigned int windowHeight);
		void Reset(glm::vec2 position, glm::vec2 velocity);
	};
}