#pragma once

#include "FrameBuffer.h"
#include "Scene.h"

namespace scene {
	class SceneFrameBuffer : public Scene {
	public:
		SceneFrameBuffer(GLFWwindow*& window);
		~SceneFrameBuffer();

		void OnRender() override;

	private:
		std::unique_ptr<FrameBuffer> m_FBO;
	};
}