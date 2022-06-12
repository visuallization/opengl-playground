#pragma once

#include "glm/glm.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera {
public:
	Camera(GLFWwindow*& window, glm::vec3 postion = glm::vec3(0.0f, 0.0f, 50.0f));
	~Camera();

	void OnUpdate(float deltaTime);
	glm::mat4 GetViewMatrix();

	float FieldOfView;

private:
	void HandleKeyboardInput(GLFWwindow* window, float deltaTime);
	void HandleMouseInput(GLFWwindow* window);

	GLFWwindow* m_Window;

	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	float m_Speed;
	float m_Yaw;
	float m_Pitch;

	float m_LastX;
	float m_LastY;

	bool m_IsMousePressed;
};
