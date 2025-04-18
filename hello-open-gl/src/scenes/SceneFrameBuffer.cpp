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

    	ResourceManager::LoadShader("assets/shaders/Inverse.shader", "Inverse");
    	ResourceManager::LoadShader("assets/shaders/GreyScale.shader", "GreyScale");
    	ResourceManager::LoadShader("assets/shaders/Sharpen.shader", "Sharpen");
    	ResourceManager::LoadShader("assets/shaders/Blur.shader", "Blur");
    	ResourceManager::LoadShader("assets/shaders/EdgeDetection.shader", "EdgeDetection");

		m_Shader = std::make_shared<Shader>("assets/shaders/Model.shader");
		m_Model = std::make_unique<Model>("assets/models/backpack/backpack.obj");

		m_PostProcessing = std::make_unique<PostProcessing>(this->m_Width, this->m_Height);
	}

	SceneFrameBuffer::~SceneFrameBuffer() {
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void SceneFrameBuffer::OnImGuiRender() {
		ImGui::Checkbox("Apply Post-Processing", &m_ApplyPostProcessing);
		if (ImGui::Button("Inverse")) {
			m_PostProcessing->SetShader("Inverse");
		}
		ImGui::SameLine();
		if (ImGui::Button("Grey scale")) {
			m_PostProcessing->SetShader("GreyScale");
		}
		ImGui::SameLine();
		if (ImGui::Button("Sharpen")) {
			m_PostProcessing->SetShader("Sharpen");
		}
		ImGui::SameLine();
		if (ImGui::Button("Blur")) {
			m_PostProcessing->SetShader("Blur");
		}
		ImGui::SameLine();
		if (ImGui::Button("Edge Detection")) {
			m_PostProcessing->SetShader("EdgeDetection");
		}

		Scene::OnImGuiRender();
	}

	void SceneFrameBuffer::OnUpdate(float deltaTime) {
		m_Camera.OnUpdate(deltaTime);

		if (m_ApplyPostProcessing) {
			m_PostProcessing->Enable();
		} else {
			m_PostProcessing->Disable();
		}
	}

	void SceneFrameBuffer::OnRender() {
		Scene::OnRender();
		Renderer renderer;

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