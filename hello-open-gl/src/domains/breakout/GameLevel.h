#pragma once

#include <map>
#include <vector>

#include "GameObject.h"

namespace breakout {

	class GameLevel {
	public:
		std::vector<GameObject> Bricks;

		GameLevel(const char* filePath, unsigned int width, unsigned int height);

		void Load(const char* filePath, unsigned int width, unsigned int height);
		void Draw(SpriteRenderer& spriteRenderer);
		bool IsCompleted();

	private:
		std::map<unsigned int, glm::vec3> m_BrickColors;

		void Init(std::vector<std::vector<unsigned int>> tiles, unsigned int width, unsigned int height);
	};

}

