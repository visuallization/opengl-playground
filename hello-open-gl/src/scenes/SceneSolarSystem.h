#pragma once

#include <list>

#include "Camera.h"
#include "Model.h"
#include "Scene.h"
#include "Shader.h"

namespace scene {
	struct Planet {
		glm::vec3 position;

		float scale;
		float mass;

		glm::vec4 color;

		glm::vec3 velocity;
		glm::vec3 acceleration;
	};

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

		float m_TimeMultiplier;

		Planet m_Sun;
		std::list<Planet> m_Planets;

		Camera m_Camera;

		void RenderPlanet(const Renderer& renderer, const Planet& planet);
	};
}


