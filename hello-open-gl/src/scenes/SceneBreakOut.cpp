#include "SceneBreakOut.h"

#include <GLFW/glfw3.h>
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include "Shader.h"

namespace scene {

	SceneBreakOut::SceneBreakOut(GLFWwindow*& window) : Scene::Scene(window)
	{
		Shader* shader = ResourceManager::LoadShader("assets/shaders/Sprite.shader", "sprite");
		glm::mat4 projection = glm::ortho(0.0f, (float)m_Width, (float)m_Height, 0.0f, -1.0f, 1.0f);

		shader->Bind();
		shader->SetUniformMat4f("u_Projection", projection);
		shader->SetUniform1i("u_Sprite", 0);

		m_SpriteRenderer = new SpriteRenderer(shader);

		// Textures
		ResourceManager::LoadTexture("src/domains/breakout/textures/background.jpg", "background");
		ResourceManager::LoadTexture("src/domains/breakout/textures/block.png", "block");
		ResourceManager::LoadTexture("src/domains/breakout/textures/block_solid.png", "block_solid");
		ResourceManager::LoadTexture("src/domains/breakout/textures/paddle.png", "paddle");

		// Levels
		m_Levels.push_back(GameLevel("src/domains/breakout/levels/1.lvl", m_Width, m_Height / 2));
		m_Levels.push_back(GameLevel("src/domains/breakout/levels/2.lvl", m_Width, m_Height / 2));
		m_Levels.push_back(GameLevel("src/domains/breakout/levels/3.lvl", m_Width, m_Height / 2));
		m_Levels.push_back(GameLevel("src/domains/breakout/levels/4.lvl", m_Width, m_Height / 2));

		m_CurrentLevel = 0;

		// Player
		
		m_Player = new GameObject(
			glm::vec2(m_Width / 2 - PLAYER_SIZE.x / 2, m_Height - PLAYER_SIZE.y),
			PLAYER_SIZE,
			ResourceManager::GetTexture("paddle")
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
			if (m_Player->Position.x > 0) {
				m_Player->Position.x -= velocity;
			}
		}

		if (glfwGetKey(m_Window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			if (m_Player->Position.x < m_Width - m_Player->Size.x) {
				m_Player->Position.x += velocity;
			}
		}

	}

	void SceneBreakOut::OnRender()
	{
		m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(m_Width, m_Height));
		m_Levels[m_CurrentLevel].Draw(*m_SpriteRenderer);
		m_Player->Draw(*m_SpriteRenderer);
	}

	void SceneBreakOut::OnImGuiRender()
	{
		ImGui::Text("Levels: "); ImGui::SameLine();
		ImGui::RadioButton("1", &m_CurrentLevel, 0); ImGui::SameLine();
		ImGui::RadioButton("2", &m_CurrentLevel, 1); ImGui::SameLine();
		ImGui::RadioButton("3", &m_CurrentLevel, 2); ImGui::SameLine();
		ImGui::RadioButton("4", &m_CurrentLevel, 3);
	}
}