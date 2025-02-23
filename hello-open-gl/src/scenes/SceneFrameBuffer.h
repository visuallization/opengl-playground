#pragma once

#include "Camera.h"
#include "FrameBuffer.h"
#include "Model.h"
#include "Scene.h"
#include "SpriteRenderer.h"

namespace scene {
	class SceneFrameBuffer : public Scene {
	public:
		SceneFrameBuffer(GLFWwindow*& window);
		~SceneFrameBuffer();
		void OnRender() override;
		void OnUpdate(float deltaTime) override;

	private:
		void Clear();

		std::unique_ptr<FrameBuffer> m_FBO;
		std::unique_ptr<RenderBuffer> m_RBO;

		std::shared_ptr<Shader> m_Shader;

		std::unique_ptr<Model> m_Model;

		SpriteRenderer* m_SpriteRenderer;

		Camera m_Camera;

		bool m_Wireframe;
	};
}