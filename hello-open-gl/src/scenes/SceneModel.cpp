#include "SceneModel.h"

#include "glm/gtc/matrix_transform.hpp"

#include "Model.h"

namespace scene {
	SceneModel::SceneModel(GLFWwindow*& window) :
		Scene(window),
		m_Model(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Projection(glm::mat4(1.0f)),
		m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f))
	{
		// Enable depth testing
		GLCall(glEnable(GL_DEPTH_TEST));

		m_Shader = std::make_unique<Shader>("res/shaders/Model.shader");
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

		// camera
		m_View = m_Camera.GetViewMatrix();

		// model
		m_Model = glm::mat4(1.0f);

		// projection
		m_Projection = glm::perspective(glm::radians(m_Camera.FieldOfView), (float)m_Width / (float)m_Height, 0.1f, 100.f);

		for (int i = 0; i < m_3DModel->Meshes.size(); i++) {
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_Model", m_Model);
			m_Shader->SetUniformMat4f("u_View", m_View);
			m_Shader->SetUniformMat4f("u_Projection", m_Projection);
			renderer.Draw(*m_3DModel->Meshes[i].VAO, *m_3DModel->Meshes[i].IBO, *m_Shader);
			m_Shader->Unbind();
		}
	}

	void SceneModel::OnImGuiRender() {}
}
