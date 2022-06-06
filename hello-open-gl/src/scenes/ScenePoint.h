#pragma once

#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "Scene.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace scene {
	class ScenePoint : public Scene {
	public:
		ScenePoint(GLFWwindow*& window);
		~ScenePoint();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::vector<float> ParsePTS(const std::string& filePath) const;

		std::unique_ptr<Shader> m_Shader;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;

		glm::mat4 m_Model;
		glm::mat4 m_View;
		glm::mat4 m_Projection;

		std::vector<float> m_Vertices;

		glm::vec3 m_Rotation;
		float m_PointSize;

		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;
		float m_CameraSpeed;
	};
}