#include "SceneMarchingCubes.h"

#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace scene {
	SceneMarchingCubes::SceneMarchingCubes(const unsigned int width, const unsigned int height) :
		m_Width(width), m_Height(height),
		m_View(glm::mat4(1.0f)),
		m_Projection(glm::mat4(1.0f)),
		m_Color(1.0f, 0.0f, 0.0f, 1.0f), m_Translation(0, 0, 0)
	{
		// Enable depth testing
		GLCall(glEnable(GL_DEPTH_TEST));

		float vertices[] = {
		-100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f, -100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		-100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,

		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,

		-100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f, -100.0f,

		-100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f, -100.0f,
		};

		// utilize indices to reuse vertices
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
		};

		m_VAO = std::make_unique<VertexArray>();

		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

		VertexBufferLayout layout;
		// add positions
		layout.Push<float>(3);
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Marching.shader");
		m_Shader->Bind();
		m_Shader->SetUniformVec4f("u_Color", m_Color);

		m_Projection = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, -1.f, 1.f);
	}

	SceneMarchingCubes::~SceneMarchingCubes() {
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void SceneMarchingCubes::OnUpdate(float deltaTime) {

	}

	void SceneMarchingCubes::OnRender() {
		Renderer renderer;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -800));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		m_View = glm::translate(glm::mat4(1.0f), m_Translation);
		glm::mat4 mvp = m_Projection * m_View * model;

		m_Shader->Bind();
		m_Shader->SetUniformVec4f("u_Color", m_Color);
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		renderer.Draw(*m_VAO, *m_IBO, *m_Shader, true, 36);
	}

	void SceneMarchingCubes::OnImGuiRender() {
		ImGui::ColorEdit4("Color", &m_Color[0]);
		ImGui::SliderFloat3("Camera", &m_Translation[0], -800, m_Width);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}