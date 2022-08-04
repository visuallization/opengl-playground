#include "SceneModel.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "Model.h"

namespace scene {
	SceneModel::SceneModel(GLFWwindow*& window) :
		Scene(window),
		m_Model(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Projection(glm::mat4(1.0f)),
		m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f)),
		m_Explode(false)
	{
		// Enable depth testing
		GLCall(glEnable(GL_DEPTH_TEST));

		m_ShaderModel = std::make_shared<Shader>("res/shaders/Model.shader");
		m_ShaderExplosion = std::make_shared<Shader>("res/shaders/Explosion.shader");
		m_Shader = m_ShaderModel;

		m_3DModel = std::make_unique<Model>("res/models/backpack/backpack.obj");
	}

	SceneModel::~SceneModel() {
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void SceneModel::OnUpdate(float deltaTime) {
		m_Camera.OnUpdate(deltaTime);
	}

	void SceneModel::OnRender() {
		Renderer renderer;

		m_Shader = m_Explode ? m_ShaderExplosion : m_ShaderModel;

		// camera
		m_View = m_Camera.GetViewMatrix();

		// model
		m_Model = glm::mat4(1.0f);

		// projection
		m_Projection = glm::perspective(glm::radians(m_Camera.FieldOfView), (float)m_Width / (float)m_Height, 0.1f, 100.f);

		// mesh
		for (int i = 0; i < m_3DModel->Meshes.size(); i++) {
			m_Shader->Bind();

			Mesh& mesh = m_3DModel->Meshes[i];

			if (mesh.Textures.size() > 0) {
				m_Shader->SetUniform1i("u_Texture", 0);
				mesh.Textures[0].Texture->Bind();
			}

			m_Shader->SetUniformMat4f("u_Model", m_Model);
			m_Shader->SetUniformMat4f("u_View", m_View);
			m_Shader->SetUniformMat4f("u_Projection", m_Projection);

			if (m_Explode) {
				m_Shader->SetUniform1f("u_Time", glfwGetTime());
			}

			renderer.Draw(*mesh.VAO, *mesh.IBO, *m_Shader);

			m_Shader->Unbind();
		}
	}

	void SceneModel::OnImGuiRender() {
		ImGui::Checkbox("Explode", &m_Explode);
	}
}
