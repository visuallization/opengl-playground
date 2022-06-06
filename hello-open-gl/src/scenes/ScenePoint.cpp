#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "VertexBufferLayout.h"

#include "ScenePoint.h"

namespace scene {
	void OnScroll(GLFWwindow* window, double xOffset, double yOffset) {
		ScenePoint* scene = reinterpret_cast<ScenePoint*>(glfwGetWindowUserPointer(window));

		if (scene) {
			scene->fieldOfView -= (float)yOffset;
			if (scene->fieldOfView < 1.0f) {
				scene->fieldOfView = 1.0f;
			}
			if (scene->fieldOfView > 45.0f) {
				scene->fieldOfView = 45.0f;
			}
		}
	}

	ScenePoint::ScenePoint(GLFWwindow*& window) :
		Scene::Scene(window),
		m_Model(glm::mat4(1.0f)),
		m_View(glm::mat4(1.0f)),
		m_Projection(glm::mat4(1.0f)),
		m_Rotation(0, 0, 0),
		m_PointSize(1.0f),
		m_CameraPosition(0.0f, 0.0f, 50.0f),
		m_CameraFront(0.0f, 0.0f, -1.0f),
		m_CameraUp(0.0f, 1.0f, 0.0f),
		m_CameraSpeed(20.0f),
		m_CameraYaw(-90.0f),
		m_CameraPitch(0.0f),
		m_LastX(m_Width / 2), m_LastY(m_Height / 2),
		m_IsMousePressed(false),
		fieldOfView(45.0f)
	{
		// Enable point sprite
		GLCall(glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));
		// vertex position + vertex color
		m_Vertices = ParsePTS("res/models/test.pts");

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

		glfwSetWindowUserPointer(m_Window, reinterpret_cast<void *>(this));
		glfwSetScrollCallback(m_Window, OnScroll);
	}

	ScenePoint::~ScenePoint() {
		GLCall(glDisable(GL_VERTEX_PROGRAM_POINT_SIZE));
	}

	void ScenePoint::OnUpdate(float deltaTime) {
		// handle keyboard input
		float speed = m_CameraSpeed * deltaTime;

		if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) {
			m_CameraPosition += speed * m_CameraFront;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
			m_CameraPosition -= speed * m_CameraFront;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
			// normalize to get a consistent movement speed independent of the camera's orientation
			m_CameraPosition -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * speed;
		}
		if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
			// normalize to get a consistent movement speed independent of the camera's orientation
			m_CameraPosition += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * speed;
		}

		// handle mouse input
		if (!m_IsMousePressed && glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_IsMousePressed = true;

			double xPos, yPos;
			glfwGetCursorPos(m_Window, &xPos, &yPos);

			m_LastX = xPos;
			m_LastY = yPos;
		}
		if (m_IsMousePressed && glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_IsMousePressed = false;
		}
		
		// handle mouse movement
		if (m_IsMousePressed) {
			double xPos, yPos;
			glfwGetCursorPos(m_Window, &xPos, &yPos);

			float xOffset = xPos - m_LastX;
			float yOffset = m_LastY - yPos;
			m_LastX = xPos;
			m_LastY = yPos;

			float sensitivity = 0.1f;
			xOffset *= sensitivity;
			yOffset *= sensitivity;

			m_CameraYaw += xOffset;
			m_CameraPitch += yOffset;

			if (m_CameraPitch > 89.0f) {
				m_CameraPitch = 89.0f;
			}
			if (m_CameraPitch < -89.0f) {
				m_CameraPitch = -89.0f;
			}

			glm::vec3 direction;
			direction.x = cos(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
			direction.y = sin(glm::radians(m_CameraPitch));
			direction.z = sin(glm::radians(m_CameraYaw)) * cos(glm::radians(m_CameraPitch));
			m_CameraFront = glm::normalize(direction);
		}
	}

	void ScenePoint::OnRender() {
		Renderer renderer;

		// camera
		m_View = glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, m_CameraUp);

		// model
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, glm::vec3(0, -15, 0));
		m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_Model = glm::rotate(m_Model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		// projection
		m_Projection = glm::perspective(glm::radians(fieldOfView), (float)m_Width / (float)m_Height, 0.1f, 100.f);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_Model", m_Model);
		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Projection", m_Projection);
		m_Shader->SetUniform1f("u_PointSize", m_PointSize);
		renderer.DrawPoints(*m_VAO, *m_Shader, m_Vertices.size() / 6);
		m_Shader->Unbind();
	}

	void ScenePoint::OnImGuiRender() {
		ImGui::SliderFloat("Point Size", &m_PointSize, 1, 64);
		ImGui::SliderFloat3("Rotation", &m_Rotation[0], 0, 360);
	}

	std::vector<float> ScenePoint::ParsePTS(const std::string& filePath) const {
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

				// the first 3 values are position coordinates
				// the last 3 values are color valus
				// map color values from 0-255 to 0-1
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