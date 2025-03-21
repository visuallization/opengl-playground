#include "ResourceManager.h"
#include "SceneTexture2D.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace scene {
	SceneTexture2D::SceneTexture2D(GLFWwindow*& window):
		Scene::Scene(window),
		m_Projection(glm::ortho(0.0f, (float)m_Width, (float)m_Height, 0.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
		// Handle transparent textures
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		// positions + texture coordinates
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f, // 0
			 50.0f, -50.0f, 1.0f, 0.0f, // 1
			 50.0f,  50.0f, 1.0f, 1.0f, // 2
			-50.0f,  50.0f, 0.0f, 1.0f  // 3
		};

		// utilize indices to reuse vertices
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
		};

		m_VAO = std::make_unique<VertexArray>();

		m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		// add positions
		layout.Push<float>(2);
		// add texture coordinates
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(indices, 6);

		Shader* shader = ResourceManager::LoadShader("assets/shaders/Basic.shader", "basic");
		shader->Bind();

		Texture* texture = ResourceManager::LoadTexture("assets/textures/emoji.png", "emoji");
		shader->SetUniform1i("u_Texture", 0);
	}

	SceneTexture2D::~SceneTexture2D() {
		GLCall(glDisable(GL_BLEND));
		ResourceManager::Clear();
	}

	void SceneTexture2D::OnUpdate(float deltaTime) {

	}

	void SceneTexture2D::OnRender() {
		Renderer renderer;
		Texture* texture = ResourceManager::GetTexture("emoji");
		texture->Bind();

		Shader* shader = ResourceManager::GetShader("basic");

		{
			// model: this simulates the object's transforms
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Projection * m_View * model;

			shader->Bind();

			// set uniforms per draw (not individual vertices)
			shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IBO, *shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Projection * m_View * model;

			shader->Bind();
			shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IBO, *shader);
		}
	}

	void SceneTexture2D::OnImGuiRender() {
		ImGui::SliderFloat2("TranslationA", &m_TranslationA.x, 0, m_Width);
		ImGui::SliderFloat2("TranslationB", &m_TranslationB.x, 0, m_Width);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}