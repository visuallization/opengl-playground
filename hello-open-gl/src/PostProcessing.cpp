#include "PostProcessing.h"
#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "ResourceManager.h"

PostProcessing::PostProcessing(int width, int height) : m_Width(width), m_Height(height), m_Active(true) {
    ResourceManager::LoadShader("assets/shaders/Sprite.shader", "Default");
    ResourceManager::LoadShader("assets/shaders/Inverse.shader", "Inverse");
    ResourceManager::LoadTexture(this->m_Width, this->m_Height, "FrameBufferTexture");

    Shader* shader = ResourceManager::GetShader("Inverse");
	Texture* texture = ResourceManager::GetTexture("FrameBufferTexture");

	glm::mat4 projection = GetProjectionMatrix();
    
    shader->Bind();
	shader->SetUniformMat4f("u_Projection", projection);
	shader->Unbind();

    m_SpriteRenderer = new SpriteRenderer(projection, true);
	m_SpriteRenderer->SetShader(shader);

    // initialize renderbuffer with a multisampled color buffer
	m_RBO = std::make_unique<RenderBuffer>(m_Width, m_Height, GL_RGB, 4);

    // initialize multisampled framebuffer
    m_MSFBO = std::make_unique<FrameBuffer>();
    m_MSFBO->AttachRenderBuffer(m_RBO->GetID(), GL_COLOR_ATTACHMENT0);

    m_FBO = std::make_unique<FrameBuffer>();
    m_FBO->AttachTexture(texture);

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

    m_MSFBO->Bind();

    Renderer renderer;
    renderer.Clear();
}

void PostProcessing::Done() {
    if (!m_Active) {
        return;
    }

    Renderer renderer;
    renderer.BlitFrameBuffer(m_MSFBO.get(), m_FBO.get(), m_Width, m_Height);
    renderer.BindDefaultFrameBuffer();

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
    glm::mat4 projection = GetProjectionMatrix();

    shader->Bind();
	shader->SetUniformMat4f("u_Projection", projection);
	shader->Unbind();

    m_SpriteRenderer->SetShader(shader);
}

void PostProcessing::SetShader(std::string shaderName) {
    SetShader(ResourceManager::GetShader(shaderName));
}

void PostProcessing::Draw() {
	m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("FrameBufferTexture"), glm::vec2(0, 0), glm::vec2(m_Width, m_Height));
}

glm::mat4 PostProcessing::GetProjectionMatrix() {
    return glm::ortho(0.0f, (float)m_Width, (float)m_Height, 0.0f, -1.0f, 1.0f);
}