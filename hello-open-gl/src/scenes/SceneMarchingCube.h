#pragma once

#include "Camera.h"
#include "Scene.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace scene {
	class SceneMarchingCube : public Scene {
	public:
		SceneMarchingCube(GLFWwindow*& window);
		~SceneMarchingCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;

		glm::mat4 m_Model;
		glm::mat4 m_View;
		glm::mat4 m_Projection;

		std::vector<float> m_Vertices;

		Camera m_Camera;
	};
}


