#include "ScenePoint.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "VertexBufferLayout.h"

namespace scene {
	ScenePoint::ScenePoint() :
		m_View(glm::mat4(1.0f)),
		m_Projection(glm::mat4(1.0f)),
		m_CameraTranslation(0, 0, 0) 
	{
		// Enable point sprite
		GLCall(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));

		// vertex position + vertex color
		float vertices[] = {
			-100.0f, -100.0f, -100.0f,
			100.0f, -100.0f, -100.0f,
			100.0f,  100.0f, -100.0f,
			-100.0f,  100.0f, -100.0f
		};

		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

		VertexBufferLayout layout;
		// add positions
		layout.Push<float>(3);

		m_VAO->AddBuffer(*m_VBO, layout);

		m_Shader = std::make_unique<Shader>("res/shaders/Point.shader");
		m_Shader->Bind();
		m_Shader->Unbind();

		float width = 960.0f;
		float height = 540.0f;
		m_Projection = glm::perspective(glm::radians(45.f), width / height, 1.f, -1.0f);
	}

	ScenePoint::~ScenePoint() {
		GLCall(glDisable(GL_VERTEX_PROGRAM_POINT_SIZE));
	}

	void ScenePoint::OnUpdate(float deltaTime) {}

	void ScenePoint::OnRender() {
		Renderer renderer;

		// camera
		m_View = glm::translate(glm::mat4(1.0f), m_CameraTranslation);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_Projection", m_Projection);
		m_Shader->SetUniformMat4f("u_View", m_View);
		renderer.DrawPoints(*m_VAO, *m_Shader, 4);
		m_Shader->Unbind();
	}

	void ScenePoint::OnImGuiRender() {
		ImGui::SliderFloat3("Camera", &m_CameraTranslation[0], -1000, 1000);
	}
}