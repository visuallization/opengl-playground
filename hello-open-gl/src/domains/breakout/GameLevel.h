#pragma once

#include <map>
#include <vector>

#include "./components/Brick.h"

namespace breakout {

	const std::map<unsigned int, glm::vec4> BRICK_COLORS = {
		{ 1, glm::vec4(1, 1, 1, 1) },
		{ 2, glm::vec4(1, 0, 0, 1) },
		{ 3, glm::vec4(0, 1, 0, 1) },
		{ 4, glm::vec4(0, 0, 1, 1) },
		{ 5, glm::vec4(1, 1, 0, 1) }
	};

	class GameLevel {
	public:
		std::vector<Brick> Bricks;

		GameLevel(const char* filePath, unsigned int width, unsigned int height);

		void Load(const char* filePath, unsigned int width, unsigned int height);
		void Draw(SpriteRenderer& renderer, bool debug = false);
		void Reset();
		bool IsCompleted();

	private:
		const char* m_FilePath;
		unsigned int m_Width;
		unsigned int m_Height;

		void Init(std::vector<std::vector<unsigned int>> tiles, unsigned int width, unsigned int height);
	};

}

