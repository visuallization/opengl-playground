#include "SceneCube.h"

#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace scene {
	SceneCube::SceneCube(GLFWwindow*& window) :
		Scene::Scene(window),
		m_View(glm::mat4(1.0f)),
		m_Projection(glm::mat4(1.0f)),
		m_Color(1.0f, 0.5f, 0.3f, 1.0f), m_LightColor(1.0f, 1.0f, 1.0f, 1.0f), m_LightPosition(300.0f, 200.0f, -800.0f), m_AmbientStrength(0.1f),
		m_CameraTranslation(0, 0, 0)
	{
		// Enable depth testing
		GLCall(glEnable(GL_DEPTH_TEST));

		// positions (3) + normals (3)
		float vertices[] = {
			-100.0f, -100.0f, -100.0f,  0.0f,  0.0f, -1.0f,
			 100.0f, -100.0f, -100.0f,  0.0f,  0.0f, -1.0f,
			 100.0f,  100.0f, -100.0f,  0.0f,  0.0f, -1.0f,
			 100.0f,  100.0f, -100.0f,  0.0f,  0.0f, -1.0f,
			-100.0f,  100.0f, -100.0f,  0.0f,  0.0f, -1.0f,
			-100.0f, -100.0f, -100.0f,  0.0f,  0.0f, -1.0f,

			-100.0f, -100.0f,  100.0f,  0.0f,  0.0f, 1.0f,
			 100.0f, -100.0f,  100.0f,  0.0f,  0.0f, 1.0f,
			 100.0f,  100.0f,  100.0f,  0.0f,  0.0f, 1.0f,
			 100.0f,  100.0f,  100.0f,  0.0f,  0.0f, 1.0f,
			-100.0f,  100.0f,  100.0f,  0.0f,  0.0f, 1.0f,
			-100.0f, -100.0f,  100.0f,  0.0f,  0.0f, 1.0f,

			-100.0f,  100.0f,  100.0f, -1.0f,  0.0f,  0.0f,
			-100.0f,  100.0f, -100.0f, -1.0f,  0.0f,  0.0f,
			-100.0f, -100.0f, -100.0f, -1.0f,  0.0f,  0.0f,
			-100.0f, -100.0f, -100.0f, -1.0f,  0.0f,  0.0f,
			-100.0f, -100.0f,  100.0f, -1.0f,  0.0f,  0.0f,
			-100.0f,  100.0f,  100.0f, -1.0f,  0.0f,  0.0f,

			 100.0f,  100.0f,  100.0f,  1.0f,  0.0f,  0.0f,
			 100.0f,  100.0f, -100.0f,  1.0f,  0.0f,  0.0f,
			 100.0f, -100.0f, -100.0f,  1.0f,  0.0f,  0.0f,
			 100.0f, -100.0f, -100.0f,  1.0f,  0.0f,  0.0f,
			 100.0f, -100.0f,  100.0f,  1.0f,  0.0f,  0.0f,
			 100.0f,  100.0f,  100.0f,  1.0f,  0.0f,  0.0f,

			-100.0f, -100.0f, -100.0f,  0.0f, -1.0f,  0.0f,
			 100.0f, -100.0f, -100.0f,  0.0f, -1.0f,  0.0f,
			 100.0f, -100.0f,  100.0f,  0.0f, -1.0f,  0.0f,
			 100.0f, -100.0f,  100.0f,  0.0f, -1.0f,  0.0f,
			-100.0f, -100.0f,  100.0f,  0.0f, -1.0f,  0.0f,
			-100.0f, -100.0f, -100.0f,  0.0f, -1.0f,  0.0f,

			-100.0f,  100.0f, -100.0f,  0.0f,  1.0f,  0.0f,
			 100.0f,  100.0f, -100.0f,  0.0f,  1.0f,  0.0f,
			 100.0f,  100.0f,  100.0f,  0.0f,  1.0f,  0.0f,
			 100.0f,  100.0f,  100.0f,  0.0f,  1.0f,  0.0f,
			-100.0f,  100.0f,  100.0f,  0.0f,  1.0f,  0.0f,
			-100.0f,  100.0f, -100.0f,  0.0f,  1.0f,  0.0f
		};

		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

		VertexBufferLayout layout;
		// add positions
		layout.Push<float>(3);
		// add normals
		layout.Push<float>(3);
		m_VAO->AddBuffer(*m_VBO, layout);

		m_Shader = std::make_unique<Shader>("assets/shaders/Cube.shader");
		m_Shader->Bind();
		m_Shader->SetUniformVec4f("u_Color", m_Color);
		m_Shader->SetUniformVec4f("u_LightColor", m_LightColor);
		m_Shader->SetUniform1f("u_AmbientStrength", m_AmbientStrength);
		m_Shader->Unbind();

		m_LightShader = std::make_unique<Shader>("assets/shaders/Light.shader");
		m_LightShader->Bind();
		m_LightShader->SetUniformVec4f("u_Color", m_LightColor);
		m_LightShader->Unbind();

		m_Projection = glm::perspective(glm::radians(45.0f), (float)m_Width / (float)m_Height, 1.f, -1.0f);
	}

	SceneCube::~SceneCube() {
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void SceneCube::OnUpdate(float deltaTime) {

	}

	void SceneCube::OnRender() {
		Renderer renderer;

		// camera
		m_View = glm::translate(glm::mat4(1.0f), m_CameraTranslation);

		// cube
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -800));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_Projection", m_Projection);
		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Model", model);
		m_Shader->SetUniformVec4f("u_Color", m_Color);
		m_Shader->SetUniformVec4f("u_LightColor", m_LightColor);
		m_Shader->SetUniformVec3f("u_LightPosition", m_LightPosition);
		renderer.DrawArrays(*m_VAO, *m_Shader, 36);
		m_Shader->Unbind();

		// light
		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPosition);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		m_LightShader->Bind();
		m_LightShader->SetUniformMat4f("u_Projection", m_Projection);
		m_LightShader->SetUniformMat4f("u_View", m_View);
		m_LightShader->SetUniformMat4f("u_Model", model);
		m_LightShader->SetUniformVec4f("u_Color", m_LightColor);
		renderer.DrawArrays(*m_VAO, *m_LightShader, 36);
		m_LightShader->Unbind();
	}

	void SceneCube::OnImGuiRender() {
		ImGui::ColorEdit4("Color", &m_Color[0]);
		ImGui::SliderFloat3("Camera", &m_CameraTranslation[0], -1000, m_Width);
		ImGui::SliderFloat3("Light", &m_LightPosition[0], -1000, m_Width);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}