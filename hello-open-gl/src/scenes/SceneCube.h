#pragma once

#include "Scene.h"

#include <memory>

#include "glm/glm.hpp"

#include "Shader.h"
#include "Texture.h"

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace scene {
	class SceneCube: public Scene {
	public:
		SceneCube(GLFWwindow*& window);
		~SceneCube() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;


	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Shader> m_LightShader;

		glm::vec4 m_Color;
		glm::vec4 m_LightColor;
		glm::vec3 m_LightPosition;
		float m_AmbientStrength;

		glm::vec3 m_CameraTranslation;
	};
}

