#include "SceneBreakOut.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include "Shader.h"

namespace scene {
	SceneBreakOut::SceneBreakOut(GLFWwindow*& window) : Scene::Scene(window)
	{
		Shader* shader = ResourceManager::LoadShader("res/shaders/Sprite.shader", "sprite");
		glm::mat4 projection = glm::ortho(0.0f, (float)m_Width, (float)m_Height, 0.0f, -1.0f, 1.0f);

		shader->Bind();
		shader->SetUniformMat4f("u_Projection", projection);
		shader->SetUniform1i("u_Sprite", 0);

		m_SpriteRenderer = new SpriteRenderer(shader);

		ResourceManager::LoadTexture("src/domains/breakout/textures/block.png", "block");
		ResourceManager::LoadTexture("src/domains/breakout/textures/block_solid.png", "block_solid");
		ResourceManager::LoadTexture("src/domains/breakout/textures/background.jpg", "background");

		this->m_Levels.push_back(breakout::GameLevel("src/domains/breakout/levels/1.lvl", this->m_Width, this->m_Height / 2));
		this->m_Levels.push_back(breakout::GameLevel("src/domains/breakout/levels/2.lvl", this->m_Width, this->m_Height / 2));
		this->m_Levels.push_back(breakout::GameLevel("src/domains/breakout/levels/3.lvl", this->m_Width, this->m_Height / 2));
		this->m_Levels.push_back(breakout::GameLevel("src/domains/breakout/levels/4.lvl", this->m_Width, this->m_Height / 2));

		this->m_CurrentLevel = 0;
	}

	SceneBreakOut::~SceneBreakOut()
	{
		ResourceManager::Clear();
	}

	void SceneBreakOut::OnUpdate(float deltaTime)
	{

	}

	void SceneBreakOut::OnRender()
	{
		m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->m_Width, this->m_Height));
		this->m_Levels[this->m_CurrentLevel].Draw(*m_SpriteRenderer);
	}

	void SceneBreakOut::OnImGuiRender()
	{
		ImGui::Text("Levels: "); ImGui::SameLine();
		ImGui::RadioButton("1", &this->m_CurrentLevel, 0); ImGui::SameLine();
		ImGui::RadioButton("2", &this->m_CurrentLevel, 1); ImGui::SameLine();
		ImGui::RadioButton("3", &this->m_CurrentLevel, 2); ImGui::SameLine();
		ImGui::RadioButton("4", &this->m_CurrentLevel, 3);
	}
}