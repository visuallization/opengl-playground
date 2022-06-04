#pragma once

#include "glm/glm.hpp"

#include "Scene.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace scene {
	class ScenePoint : public Scene {
	public:
		ScenePoint();
		~ScenePoint();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<Shader> m_Shader;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;

		glm::mat4 m_View;
		glm::mat4 m_Projection;

		glm::vec3 m_CameraTranslation;
		float m_PointSize;
	};
}