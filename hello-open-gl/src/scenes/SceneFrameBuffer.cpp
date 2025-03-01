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

		Texture* texture = ResourceManager::LoadTexture(this->m_Width, this->m_Height, "FrameBufferTexture");

		glm::mat4 projection = glm::ortho(0.0f, (float)m_Width, 0.0f, (float)m_Height, -1.0f, 1.0f);

		m_PostProcessingShader = std::make_shared<Shader>("assets/shaders/Inverse.shader");
		m_PostProcessingShader->Bind();
		m_PostProcessingShader->SetUniformMat4f("u_Projection", projection);
		m_PostProcessingShader->Unbind();

		m_SpriteRenderer = new SpriteRenderer(projection);
		m_SpriteRenderer->SetShader(m_PostProcessingShader.get());

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

	void SceneFrameBuffer::OnUpdate(float deltaTime) {
		m_Camera.OnUpdate(deltaTime);
	}

	void SceneFrameBuffer::OnRender() {
		Scene::OnRender();

		// first render pass
		m_FBO->Bind();
        // make sure we clear the framebuffer's content
		
		Renderer renderer;
        renderer.Clear();

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

		// second render pass
		m_FBO->Unbind();
		renderer.Clear();

		m_SpriteRenderer->Debug(Scene::m_Debug);
		m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("FrameBufferTexture"), glm::vec2(0, 0), glm::vec2(m_Width, m_Height));
	}
}