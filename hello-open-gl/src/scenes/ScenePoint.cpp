#include "ScenePoint.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "VertexBufferLayout.h"

namespace scene {
	ScenePoint::ScenePoint() :
		m_View(glm::mat4(1.0f)),
		m_Projection(glm::mat4(1.0f)),
		m_CameraTranslation(0, 0, 0),
		m_PointSize(1.0f)
	{
		// Enable point sprite
		GLCall(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));

		// vertex position + vertex color
		m_Vertices = ParsePTS("res/models/model.pts");

		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(float));

		VertexBufferLayout layout;
		// add positions
		layout.Push<float>(3);
		// add color
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
		m_Shader->SetUniform1f("u_PointSize", m_PointSize);
		renderer.DrawPoints(*m_VAO, *m_Shader, m_Vertices.size() / 6);
		m_Shader->Unbind();
	}

	void ScenePoint::OnImGuiRender() {
		ImGui::SliderFloat("Point Size", &m_PointSize, 0, 100);
		ImGui::SliderFloat3("Camera", &m_CameraTranslation[0], -100, 100);
	}

	std::vector<float> scene::ScenePoint::ParsePTS(const std::string& filePath) const
	{
		std::vector<float> vertices;

		std::ifstream stream(filePath);
		std::string line;

		while (getline(stream, line))
		{
			std::stringstream ss(line);
			std::string value;
			int i = 1;

			while (ss >> value) {
				float number = std::stof(value);

				if (i > 3) {
					number = number / 255.0f;
				}

				vertices.push_back(number);
				i++;
			}
		}

		return vertices;
	}
}