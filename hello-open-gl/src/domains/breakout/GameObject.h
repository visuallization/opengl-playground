#pragma once

#include "glm/gtc/matrix_transform.hpp"

#include "Texture.h"
#include "SpriteRenderer.h"

namespace breakout {

	class GameObject {
	public:
		glm::vec2 Position;
		glm::vec2 Size;
		glm::vec2 Velocity;

		glm::vec3 Color;

		float Rotation;

		bool IsDestroyable;
		bool IsActive;

		Texture* Sprite;

		GameObject();
		GameObject(glm::vec2 position, glm::vec2 size, Texture* sprite, float rotation = 0, glm::vec3 color = glm::vec3(1.0f), bool IsDestroyable = true, glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

		virtual void Draw(SpriteRenderer& spriteRenderer);
	};

}

