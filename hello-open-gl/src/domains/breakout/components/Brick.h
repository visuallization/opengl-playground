#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"

#include "../GameObject.h"

namespace breakout {
	class Brick : public GameObject {
	public:
		Brick();
		Brick(glm::vec2 position, glm::vec2 size, Texture* sprite, bool IsSolid = false, glm::vec3 color = glm::vec3(1.0f), float rotation = 0);

		bool IsSolid;
	};
}