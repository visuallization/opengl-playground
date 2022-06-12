#include "SceneMarchingCube.h"

#include "glm/gtc/matrix_transform.hpp"

namespace scene {
	SceneMarchingCube::SceneMarchingCube(GLFWwindow*& window) :
		Scene(window),
		m_Model(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Projection(glm::mat4(1.0f)),
		m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f))
	{
		m_Vertices = { 
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(float));

		VertexBufferLayout layout;
		// add positions
		layout.Push<float>(3);

		m_VAO->AddBuffer(*m_VBO, layout);

		m_Shader = std::make_unique<Shader>("res/shaders/MarchingCube.shader");
	}

	SceneMarchingCube::~SceneMarchingCube() {}

	void SceneMarchingCube::OnUpdate(float deltaTime) {
		m_Camera.OnUpdate(deltaTime);
	}

	void SceneMarchingCube::OnRender() {
		Renderer renderer;

		// camera
		m_View = m_Camera.GetViewMatrix();

		// model
		m_Model = glm::mat4(1.0f);

		// projection
		m_Projection = glm::perspective(glm::radians(m_Camera.FieldOfView), (float)m_Width / (float)m_Height, 0.1f, 100.f);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_Model", m_Model);
		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Projection", m_Projection);
		renderer.DrawPoints(*m_VAO, *m_Shader, m_Vertices.size() / 3);
		m_Shader->Unbind();
	}

	void SceneMarchingCube::OnImGuiRender() {}
}
