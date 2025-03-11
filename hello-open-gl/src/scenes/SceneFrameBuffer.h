#pragma once

#include "Camera.h"
#include "Model.h"
#include "PostProcessing.h"
#include "Scene.h"

namespace scene {
	class SceneFrameBuffer : public Scene {
	public:
		SceneFrameBuffer(GLFWwindow*& window);
		~SceneFrameBuffer();
		void OnRender() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;

	private:
		std::shared_ptr<Shader> m_Shader;
		std::unique_ptr<Model> m_Model;
		
		Camera m_Camera;
		
		std::unique_ptr<PostProcessing> m_PostProcessing;
		bool m_ApplyPostProcessing = false;
	};
}