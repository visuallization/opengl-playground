#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

void OnScroll(GLFWwindow* window, double xOffset, double yOffset) {
	Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));

	// zooming
	if (camera) {
		camera->FieldOfView -= (float)yOffset;
		if (camera->FieldOfView < 1.0f) {
			camera->FieldOfView = 1.0f;
		}
		if (camera->FieldOfView > 45.0f) {
			camera->FieldOfView = 45.0f;
		}
	}
}

Camera::Camera(GLFWwindow*& window, glm::vec3 postion) :
	m_Window(window),
	m_Position(postion),
	m_Front(0.0f, 0.0f, -1.0f),
	m_Up(0.0f, 1.0f, 0.0f),
	m_Speed(20.0f),
	m_Yaw(-90.0f),
	m_Pitch(0.0f),
	m_LastX(0), m_LastY(0),
	m_IsMousePressed(false),
	FieldOfView(45.0f)
{
	glfwSetWindowUserPointer(m_Window, reinterpret_cast<void*>(this));
	glfwSetScrollCallback(m_Window, OnScroll);
}

Camera::~Camera() {}

void Camera::OnUpdate(float deltaTime) {
	HandleKeyboardInput(m_Window, deltaTime);
	HandleMouseInput(m_Window);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::HandleKeyboardInput(GLFWwindow* window, float deltaTime) {
	float speed = m_Speed * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		m_Position += speed * m_Front;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		m_Position -= speed * m_Front;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		// normalize to get a consistent movement speed independent of the camera's orientation
		m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		// normalize to get a consistent movement speed independent of the camera's orientation
		m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * speed;
	}
}

void Camera::HandleMouseInput(GLFWwindow* window) {
	// handle mouse buttons
	if (!m_IsMousePressed && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_IsMousePressed = true;

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		m_LastX = xPos;
		m_LastY = yPos;
	}
	if (m_IsMousePressed && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_IsMousePressed = false;
	}

	// handle mouse movement
	if (m_IsMousePressed) {
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		float xOffset = xPos - m_LastX;
		float yOffset = m_LastY - yPos;
		m_LastX = xPos;
		m_LastY = yPos;

		float sensitivity = 0.1f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		m_Yaw += xOffset;
		m_Pitch += yOffset;

		if (m_Pitch > 89.0f) {
			m_Pitch = 89.0f;
		}
		if (m_Pitch < -89.0f) {
			m_Pitch = -89.0f;
		}

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(direction);
	}
}
