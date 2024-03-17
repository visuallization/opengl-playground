#include "GameLevel.h"
#include "ResourceManager.h"

#include <string>
#include <fstream>
#include <sstream>

namespace breakout {

	GameLevel::GameLevel(const char* filePath, unsigned int width, unsigned int height) {
		m_FilePath = filePath;
		m_Width = width;
		m_Height = height;

		Load(filePath, width, height);
	}

	void GameLevel::Load(const char* filePath, unsigned int width, unsigned int height) {
		Bricks.clear();

		unsigned int tile;
		std::string line;
		std::ifstream fstream(filePath);
		std::vector<std::vector<unsigned int>> tiles;

		if (fstream) {
			while (std::getline(fstream, line)) {
				std::istringstream sstream(line);
				std::vector<unsigned int> row;
				while (sstream >> tile) {
					row.push_back(tile);
				}
				tiles.push_back(row);
			}

			if (tiles.size() > 0) {
				Init(tiles, width, height);
			}
		}
	}

	void GameLevel::Reset() {
		Load(m_FilePath, m_Width, m_Height);
	}

	void GameLevel::Draw(SpriteRenderer& renderer, bool debug /* = false */) {
		for (Brick& brick : Bricks) {
			if (brick.IsActive) {
				brick.Draw(renderer, debug);
			}
		}
	}

	bool GameLevel::IsCompleted() {
		for (Brick& brick : Bricks) {
			if (!brick.IsSolid && brick.IsActive) {
				return false;
			}
		}
		return true;
	}

	void GameLevel::Init(std::vector<std::vector<unsigned int>> tiles, unsigned int width, unsigned int height) {
		unsigned int rowCount = tiles.size();
		unsigned int columnCount = tiles[0].size();

		float tileWidth = width / columnCount;
		float tileHeight = height / rowCount;

		for (unsigned int y = 0; y < rowCount; y++) {
			for (unsigned int x = 0; x < columnCount; x++) {
				unsigned int tile = tiles[y][x];

				// Empty tile
				if (tile == 0) {
					// Do nothing
				}
				// Solid tile
				else if (tile == 1) {
					glm::vec2 position(tileWidth * x, tileHeight * y);
					glm::vec2 size(tileWidth, tileHeight);
					Brick brick(position, size, ResourceManager::GetTexture("block_solid"), true, BRICK_COLORS.at(tile));
					Bricks.push_back(brick);
				}
				// Destroyable tiles
				else if (tile > 1) {
					glm::vec2 position(tileWidth * x, tileHeight * y);
					glm::vec2 size(tileWidth, tileHeight);
					Brick brick(position, size, ResourceManager::GetTexture("block"), false, BRICK_COLORS.at(tile));
					Bricks.push_back(brick);
				}
			}
		}
	}

}