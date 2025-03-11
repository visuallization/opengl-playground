#include "PostProcessing.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "ResourceManager.h"

PostProcessing::PostProcessing(int width, int height) : m_Width(width), m_Height(height), m_Active(true) {
    ResourceManager::LoadShader("assets/shaders/Sprite.shader", "Default");
    ResourceManager::LoadShader("assets/shaders/Inverse.shader", "Inverse");
    ResourceManager::LoadTexture(this->m_Width, this->m_Height, "FrameBufferTexture");

    Shader* shader = ResourceManager::GetShader("Inverse");
	Texture* texture = ResourceManager::GetTexture("FrameBufferTexture");

	glm::mat4 projection = glm::ortho(0.0f, (float)m_Width, (float)m_Height, 0.0f, -1.0f, 1.0f);
    
    shader->Bind();
	shader->SetUniformMat4f("u_Projection", projection);
	shader->Unbind();

    m_SpriteRenderer = new SpriteRenderer(projection, true);
	m_SpriteRenderer->SetShader(shader);

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

void PostProcessing::Start() {
    if (!m_Active) {
        return;
    }

    m_FBO->Bind();

    Renderer renderer;
    renderer.Clear();
}

void PostProcessing::Done() {
    if (!m_Active) {
        return;
    }

    m_FBO->Unbind();

    Renderer renderer;
    renderer.Clear();

    Draw();
}

void PostProcessing::Enable() {
    m_Active = true;
}

void PostProcessing::Disable() {
    m_Active = false;
    m_FBO->Unbind();
}

void PostProcessing::SetShader(Shader* shader) {
    glm::mat4 projection = glm::ortho(0.0f, (float)m_Width, (float)m_Height, 0.0f, -1.0f, 1.0f);

    shader->Bind();
	shader->SetUniformMat4f("u_Projection", projection);
	shader->Unbind();

    m_SpriteRenderer->SetShader(shader);
}

void PostProcessing::Draw() {
	m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("FrameBufferTexture"), glm::vec2(0, 0), glm::vec2(m_Width, m_Height));
}