#pragma once

#include <map>
#include <vector>

#include "./components/Brick.h"

namespace breakout {

	const std::map<unsigned int, glm::vec3> BRICK_COLORS = {
		{ 1, glm::vec3(1, 1, 1) },
		{ 2, glm::vec3(1, 0, 0) },
		{ 3, glm::vec3(0, 1, 0) },
		{ 4, glm::vec3(0, 0, 1) },
		{ 5, glm::vec3(1, 1, 0) }
	};

	class GameLevel {
	public:
		std::vector<Brick> Bricks;

		GameLevel(const char* filePath, unsigned int width, unsigned int height);

		void Load(const char* filePath, unsigned int width, unsigned int height);
		void Draw(SpriteRenderer& renderer, bool debug = false);
		bool IsCompleted();

	private:
		void Init(std::vector<std::vector<unsigned int>> tiles, unsigned int width, unsigned int height);
	};

}

