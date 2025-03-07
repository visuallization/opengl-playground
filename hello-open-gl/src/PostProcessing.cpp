#include "PostProcessing.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "ResourceManager.h"

PostProcessing::PostProcessing(int width, int height) : m_Width(width), m_Height(height) {
	Texture* texture = ResourceManager::LoadTexture(this->m_Width, this->m_Height, "FrameBufferTexture");
	glm::mat4 projection = glm::ortho(0.0f, (float)m_Width, (float)m_Height, 0.0f, -1.0f, 1.0f);
    
	m_Shader = std::make_shared<Shader>("assets/shaders/Inverse.shader");
    m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_Projection", projection);
	m_Shader->Unbind();

    m_SpriteRenderer = new SpriteRenderer(projection, true);
	m_SpriteRenderer->SetShader(m_Shader.get());

	m_RBO = std::make_unique<RenderBuffer>(m_Width, m_Height);

    m_FBO = std::make_unique<FrameBuffer>();
    m_FBO->AttachTexture(texture);
	m_FBO->AttachRenderBuffer(m_RBO->GetID());

    if (m_FBO->IsComplete()) {
        std::cout << "FrameBuffer complete. Ready to render.";
    }
    else {
        std::cout << "FrameBuffer incomplete";
    }

    m_FBO->Unbind();
}

PostProcessing::~PostProcessing() {
}

void PostProcessing::Bind() {
    m_FBO->Bind();

    Renderer renderer;
    renderer.Clear();
}

void PostProcessing::Unbind() {
    m_FBO->Unbind();

    Renderer renderer;
    renderer.Clear();
}

void PostProcessing::Draw() {
	m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("FrameBufferTexture"), glm::vec2(0, 0), glm::vec2(m_Width, m_Height));
}