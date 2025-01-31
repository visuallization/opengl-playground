#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "SceneFrameBuffer.h"
#include "ResourceManager.h"
#include "Texture.h"

namespace scene {
	SceneFrameBuffer::SceneFrameBuffer(GLFWwindow*& window) :
		Scene::Scene(window),
		m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f))
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_Shader = std::make_shared<Shader>("assets/shaders/Model.shader");
		m_Model = std::make_unique<Model>("assets/models/backpack/backpack.obj");

		m_FBO = std::make_unique<FrameBuffer>();
		m_RBO = std::make_unique<RenderBuffer>(this->m_Width, this->m_Height);

		// Check with how many channels you initialize the empty texture (for color you probably only need 8 bit)
		Texture* texture = ResourceManager::LoadTexture(this->m_Width, this->m_Height, "FrameBufferTexture");
		// Instantiate empty texture which we can add to the SpriteRenderer because we only need an empty quad
		ResourceManager::LoadTexture(1, 1, "empty");
		ResourceManager::LoadTexture("src/domains/breakout/assets/textures/background.jpg", "background");

		glm::mat4 projection = glm::ortho(0.0f, (float)m_Width, (float)m_Height, 0.0f, -1.0f, 1.0f);
		m_SpriteRenderer = new SpriteRenderer(projection);

		m_FBO->AttachTexture(texture);
		m_FBO->AttachRenderBuffer(m_RBO->GetID());

		if (m_FBO->IsComplete()) {
			std::cout << "FrameBuffer complete. Ready to render.";
		}
		else {
			std::cout << "FrameBuffer incomplete";
		}

		m_FBO->Unbind();
	}

	SceneFrameBuffer::~SceneFrameBuffer() {
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void SceneFrameBuffer::OnImGuiRender() {
		ImGui::Checkbox("Wireframe", &m_Wireframe);
	}

	void SceneFrameBuffer::OnUpdate(float deltaTime) {
		m_Camera.OnUpdate(deltaTime);
	}

	void SceneFrameBuffer::OnRender() {
		if (m_Wireframe) {
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		} else {
			GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		}

		Renderer renderer;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = m_Camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(m_Camera.FieldOfView), (float)m_Width / (float)m_Height, 0.1f, 100.f);

		for (int i = 0; i < m_Model->Meshes.size(); i++) {
			m_Shader->Bind();

			Mesh& mesh = m_Model->Meshes[i];

			if (mesh.Textures.size() > 0) {
				m_Shader->SetUniform1i("u_Texture", 0);
				mesh.Textures[0].Texture->Bind();
			}

			m_Shader->SetUniformMat4f("u_Model", model);
			m_Shader->SetUniformMat4f("u_View", view);
			m_Shader->SetUniformMat4f("u_Projection", projection);

			renderer.Draw(*mesh.VAO, *mesh.IBO, *m_Shader);

			m_Shader->Unbind();
		}

		m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(m_Width, m_Height));
	}
}