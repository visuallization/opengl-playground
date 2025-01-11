#include "SceneFrameBuffer.h"
#include "ResourceManager.h"
#include "Texture.h"

namespace scene {
	SceneFrameBuffer::SceneFrameBuffer(GLFWwindow*& window) :
		Scene::Scene(window)
	{
		m_FBO = std::make_unique<FrameBuffer>();

		Texture* texture = ResourceManager::LoadTexture(this->m_Width, this->m_Height, "FrameBufferTexture");
		m_FBO->AttachTexture(texture);

		if (m_FBO->IsComplete()) {
			std::cout << "FrameBuffer complete. Ready to render.";
			m_FBO->Bind();
		}
		else {
			std::cout << "FrameBuffer incomplete";
		}
	}

	SceneFrameBuffer::~SceneFrameBuffer() {

	}

	void SceneFrameBuffer::OnRender() {
		
	}
}