#include "GameLevel.h"
#include "ResourceManager.h"

#include <string>
#include <fstream>
#include <sstream>

namespace breakout {

	GameLevel::GameLevel(const char* filePath, unsigned int width, unsigned int height) {
		m_BrickColors = {
			{ 1, glm::vec3(1) },
			{ 2, glm::vec3(1, 0, 0) },
			{ 3, glm::vec3(0, 1, 0) },
			{ 4, glm::vec3(0, 0, 1) },
			{ 5, glm::vec3(1, 1, 0) }
		};

		this->Load(filePath, width, height);
	}

	void GameLevel::Load(const char* filePath, unsigned int width, unsigned int height) {
		this->Bricks.clear();

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
				this->Init(tiles, width, height);
			}
		}
	}

	void GameLevel::Draw(SpriteRenderer& spriteRenderer) {
		for (GameObject& tile : this->Bricks) {
			if (tile.IsActive) {
				tile.Draw(spriteRenderer);
			}
		}
	}

	bool GameLevel::IsCompleted() {
		for (GameObject& tile : this->Bricks) {
			if (tile.IsDestroyable && tile.IsActive) {
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
					glm::vec3 color(0.8f, 0.8f, 0.7f);
					GameObject brick(position, size, ResourceManager::GetTexture("block_solid"), 0, m_BrickColors[tile], false);
					this->Bricks.push_back(brick);
				}
				// Destroyable tiles
				else if (tile > 1) {
					glm::vec2 position(tileWidth * x, tileHeight * y);
					glm::vec2 size(tileWidth, tileHeight);
					glm::vec3 color(1.0f);
					GameObject brick(position, size, ResourceManager::GetTexture("block"), 0, m_BrickColors[tile], true);
					this->Bricks.push_back(brick);
				}
			}
		}
	}

}