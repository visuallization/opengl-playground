#include "SceneFrameBuffer.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "ResourceManager.h"
#include "Texture.h"

namespace scene {
	SceneFrameBuffer::SceneFrameBuffer(GLFWwindow*& window) :
		Scene::Scene(window),
		m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f))
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_PostProcessing = std::make_unique<PostProcessing>(this->m_Width, this->m_Height);
		m_Shader = std::make_shared<Shader>("assets/shaders/Model.shader");
		m_Model = std::make_unique<Model>("assets/models/backpack/backpack.obj");
	}

	SceneFrameBuffer::~SceneFrameBuffer() {
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void SceneFrameBuffer::OnImGuiRender() {
		Scene::OnImGuiRender();
	}

	void SceneFrameBuffer::OnUpdate(float deltaTime) {
		m_Camera.OnUpdate(deltaTime);
	}

	void SceneFrameBuffer::OnRender() {
		Renderer renderer;
		Scene::OnRender();

		m_PostProcessing->Start();

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

		m_PostProcessing->Done();
	}
}