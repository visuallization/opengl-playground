#pragma once

#include "Scene.h"
#include "SpriteRenderer.h"

#include "domains/breakout/GameLevel.h"
#include "domains/breakout/GameObject.h"

using namespace breakout;

namespace scene {
	enum GameState {
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN
	};

	const float PLAYER_VELOCITY = 500;
	const glm::vec2 PLAYER_SIZE(100, 20);

	class SceneBreakOut : public Scene {
	public:
		SceneBreakOut(GLFWwindow*& window);
		~SceneBreakOut();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		SpriteRenderer* m_SpriteRenderer;

		int m_CurrentLevel = 0;
		std::vector<GameLevel> m_Levels;

		GameObject* m_Player;
	};
}
