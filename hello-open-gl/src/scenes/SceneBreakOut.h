#pragma once

#include "PostProcessing.h"
#include "Scene.h"
#include "SpriteRenderer.h"

#include "domains/breakout/GameLevel.h"
#include "domains/breakout/GameObject.h"
#include "domains/breakout/ParticleEmitter.h"
#include "domains/breakout/components/Ball.h"

using namespace breakout;

namespace scene {
	enum GameState {
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN
	};

	const float PLAYER_VELOCITY = 500.f;
	const glm::vec2 PLAYER_SIZE(100, 20);

	const glm::vec2 INITIAL_BALL_VELOCITY(100, -350);
	const float BALL_RADIUS = 12.f;

	class SceneBreakOut : public Scene {
	public:
		SceneBreakOut(GLFWwindow*& window);
		~SceneBreakOut();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		SpriteRenderer* m_SpriteRenderer;

		std::unique_ptr<PostProcessing> m_PostProcessing;
		bool m_ApplyPostProcessing = false;

		bool m_Debug = false;
		int m_CurrentLevel = 0;
		std::vector<GameLevel> m_Levels;
		float m_ShakeTime = 0.0f;

		GameObject* m_Player;
		Ball* m_Ball;
		ParticleEmitter* m_ParticleEmitter;

		void CheckCollisions();
		void Reset();
		void Shake(bool shake = true);
	};
}
