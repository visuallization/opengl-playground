#pragma once

#include "Scene.h"
#include "SpriteRenderer.h"

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
	};
}