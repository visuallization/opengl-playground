#pragma once

#include "FrameBuffer.h"
#include "Model.h"
#include "Scene.h"

namespace scene {
	class SceneFrameBuffer : public Scene {
	public:
		SceneFrameBuffer(GLFWwindow*& window);
		~SceneFrameBuffer();
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<FrameBuffer> m_FBO;
		std::unique_ptr<RenderBuffer> m_RBO;

		std::shared_ptr<Shader> m_Shader;

		std::unique_ptr<Model> m_Model;

		bool m_Wireframe;
	};
}