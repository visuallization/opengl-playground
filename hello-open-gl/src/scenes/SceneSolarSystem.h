#pragma once

#include "Camera.h"
#include "Model.h"
#include "Scene.h"
#include "Shader.h"

namespace scene {
	class SceneSolarSystem : public Scene {
	public:
		SceneSolarSystem(GLFWwindow*& window);
		~SceneSolarSystem();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::shared_ptr<Shader> m_Shader;
		std::unique_ptr<Model> m_3DModel;

		glm::mat4 m_Model;
		glm::mat4 m_View;
		glm::mat4 m_Projection;

		Camera m_Camera;

		void RenderPlanet(const Renderer& renderer, const glm::vec3& translation, const float scale, const glm::vec4& color);
	};
}


