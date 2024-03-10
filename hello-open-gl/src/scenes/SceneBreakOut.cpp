#include "SceneBreakOut.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

#include "Renderer.h"
#include "ResourceManager.h"
#include "Shader.h"

#include "domains/breakout/Physics.h"

namespace scene {

	SceneBreakOut::SceneBreakOut(GLFWwindow*& window) : Scene::Scene(window)
	{
		glm::mat4 projection = glm::ortho(0.0f, (float)m_Width, (float)m_Height, 0.0f, -1.0f, 1.0f);
		
		Shader* shader = ResourceManager::LoadShader("assets/shaders/Sprite.shader", "sprite");
		shader->Bind();
		shader->SetUniformMat4f("u_Projection", projection);
		shader->SetUniform1i("u_Sprite", 0);
		shader->Unbind();

		Shader* debugShader = ResourceManager::LoadShader("assets/shaders/Color.shader", "debug");
		debugShader->Bind();
		debugShader->SetUniformMat4f("u_Projection", projection);
		debugShader->Unbind();

		m_SpriteRenderer = new SpriteRenderer(shader, debugShader);

		// textures
		ResourceManager::LoadTexture("src/domains/breakout/assets/textures/background.jpg", "background");
		ResourceManager::LoadTexture("src/domains/breakout/assets/textures/ball.png", "ball");
		ResourceManager::LoadTexture("src/domains/breakout/assets/textures/block.png", "block");
		ResourceManager::LoadTexture("src/domains/breakout/assets/textures/block_solid.png", "block_solid");
		ResourceManager::LoadTexture("src/domains/breakout/assets/textures/paddle.png", "paddle");

		// levels
		m_Levels.push_back(GameLevel("src/domains/breakout/assets/levels/1.lvl", m_Width, m_Height / 2));
		m_Levels.push_back(GameLevel("src/domains/breakout/assets/levels/2.lvl", m_Width, m_Height / 2));
		m_Levels.push_back(GameLevel("src/domains/breakout/assets/levels/3.lvl", m_Width, m_Height / 2));
		m_Levels.push_back(GameLevel("src/domains/breakout/assets/levels/4.lvl", m_Width, m_Height / 2));

		m_CurrentLevel = 0;

		// player
		m_Player = new GameObject(
			glm::vec2(m_Width / 2 - PLAYER_SIZE.x / 2, m_Height - PLAYER_SIZE.y),
			PLAYER_SIZE,
			ResourceManager::GetTexture("paddle")
		);

		// ball
		m_Ball = new Ball(
			m_Player->GetPosition() + glm::vec2(m_Player->Size.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2),
			BALL_RADIUS,
			INITIAL_BALL_VELOCITY,
			ResourceManager::GetTexture("ball")
		);
	}

	SceneBreakOut::~SceneBreakOut()
	{
		ResourceManager::Clear();
		delete m_Player;
		delete m_SpriteRenderer;
	}

	void SceneBreakOut::OnUpdate(float deltaTime)
	{
		float velocity = PLAYER_VELOCITY * deltaTime;


		if (glfwGetKey(m_Window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			if (m_Player->GetPosition().x > 0) {
				m_Player->UpdatePosition(glm::vec2(-velocity, 0));

				// move the ball with the player, if it is still stuck
				if (m_Ball->IsStuck) {
					m_Ball->UpdatePosition(glm::vec2(-velocity, 0));
				}
			}
		}

		if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			if (m_Player->GetPosition().x < m_Width - m_Player->Size.x) {
				m_Player->UpdatePosition(glm::vec2(velocity, 0));

				// move the ball with the player, if it is still stuck
				if (m_Ball->IsStuck) {
					m_Ball->UpdatePosition(glm::vec2(velocity, 0));
				}
			}
		}

		if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			m_Ball->IsStuck = false;
		}

		m_Ball->Move(deltaTime, m_Width, m_Height);

		CheckCollisions();
	}

	void SceneBreakOut::OnRender()
	{
		m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(m_Width, m_Height));
		m_Levels[m_CurrentLevel].Draw(*m_SpriteRenderer, true);
		m_Player->Draw(*m_SpriteRenderer, true);
		m_Ball->Draw(*m_SpriteRenderer, true);
	}

	void SceneBreakOut::OnImGuiRender()
	{
		ImGui::Text("Levels: "); ImGui::SameLine();
		ImGui::RadioButton("1", &m_CurrentLevel, 0); ImGui::SameLine();
		ImGui::RadioButton("2", &m_CurrentLevel, 1); ImGui::SameLine();
		ImGui::RadioButton("3", &m_CurrentLevel, 2); ImGui::SameLine();
		ImGui::RadioButton("4", &m_CurrentLevel, 3);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	}

	void SceneBreakOut::CheckCollisions()
	{
		for (Brick& brick : m_Levels[m_CurrentLevel].Bricks) {
			if (brick.IsActive) {
				Collision collision = Physics::IsColliding(*m_Ball->GetCollider(), *brick.GetCollider());
				if (collision.IsActive) {
					if (!brick.IsSolid) {
						brick.Destroy();
					}

					// COLLISION RESOLUTION
					// Horizontal collision
					if (collision.Orientation == Direction::LEFT || collision.Orientation == Direction::RIGHT) {
						m_Ball->Velocity.x *= -1;

						float penetration = m_Ball->Radius - std::abs(collision.Penetration.x);
						
						if (collision.Orientation == Direction::LEFT) {
							m_Ball->UpdatePosition(glm::vec2(penetration, 0));
						}
						else {
							m_Ball->UpdatePosition(glm::vec2(-penetration, 0));
						}
					}
					// Vertical collision
					else {
						m_Ball->Velocity.y *= -1;

						float penetration = m_Ball->Radius - std::abs(collision.Penetration.y);

						if (collision.Orientation == Direction::UP) {
							m_Ball->UpdatePosition(glm::vec2(0, -penetration));
						}
						else {
							m_Ball->UpdatePosition(glm::vec2(0, penetration));
						}
					}
				}
			}
		}
	}
}