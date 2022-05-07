#include "SceneMarchingCubes.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace scene {
	SceneMarchingCubes::SceneMarchingCubes(const unsigned int width, const unsigned int height) :
		m_Width(width), m_Height(height),
		m_Projection(glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f)),
		m_View(glm::mat4(1.0f)),
		m_Color(1.0f, 0.0f, 0.0f, 1.0f), m_Translation(width / 2, height / 2, 0)
	{

		float positions[] = {
			-50.0f, -50.0f,
			 50.0f, -50.0f,
			 50.0f,  50.0f,
			-50.0f,  50.0f,
		};

		// utilize indices to reuse vertices
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
		};

		m_VAO = std::make_unique<VertexArray>();

		m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 2 * sizeof(float));

		VertexBufferLayout layout;
		// add positions
		layout.Push<float>(2);
		m_VAO->AddBuffer(*m_VBO, layout);

		m_IBO = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Marching.shader");
		m_Shader->Bind();
		m_Shader->SetUniformVec4f("u_Color", m_Color);
	}

	SceneMarchingCubes::~SceneMarchingCubes() {

	}

	void SceneMarchingCubes::OnUpdate(float deltaTime) {

	}

	void SceneMarchingCubes::OnRender() {
		Renderer renderer;

		glm::mat4 model = glm::mat4(1.0f);
		m_View = glm::translate(glm::mat4(1.0f), m_Translation);
		glm::mat4 mvp = m_Projection * m_View * model;

		m_Shader->Bind();
		m_Shader->SetUniformVec4f("u_Color", m_Color);
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
	}

	void SceneMarchingCubes::OnImGuiRender() {
		ImGui::ColorEdit4("Color", &m_Color[0]);
		ImGui::SliderFloat2("Camera", &m_Translation[0], 0, m_Width);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}