#pragma once

#include "Scene.h"
#include "SpriteRenderer.h"

#include "domains/breakout/GameLevel.h"

namespace scene {
	enum GameState {
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN
	};

	class SceneBreakOut : public Scene {
	public:
		SceneBreakOut(GLFWwindow*& window);
		~SceneBreakOut();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		SpriteRenderer* m_SpriteRenderer;
		std::vector<breakout::GameLevel> m_Levels;
		int m_CurrentLevel = 0;
	};
}