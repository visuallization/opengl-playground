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

		m_SpriteRenderer = new SpriteRenderer(shader);

		ResourceManager::LoadTexture("res/textures/emoji.png", "emoji");
		shader->SetUniform1i("u_Sprite", 0);
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
		m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("emoji"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void SceneBreakOut::OnImGuiRender()
	{

	}
}