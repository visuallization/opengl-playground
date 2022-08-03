#pragma once

#include "Camera.h"
#include "Model.h"
#include "Scene.h"
#include "Shader.h"

namespace scene {
	class SceneModel : public Scene {
	public:
		SceneModel(GLFWwindow*& window);
		~SceneModel();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Model> m_3DModel;

		glm::mat4 m_Model;
		glm::mat4 m_View;
		glm::mat4 m_Projection;

		Camera m_Camera;
	};
}


