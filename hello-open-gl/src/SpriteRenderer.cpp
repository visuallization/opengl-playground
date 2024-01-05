#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "SpriteRenderer.h"
#include "VertexBufferLayout.h"

SpriteRenderer::SpriteRenderer(Shader* shader) {
	// Handle transparent textures
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	m_Shader = shader;
	initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
	// Disable blending
	GLCall(glDisable(GL_BLEND));
}

void SpriteRenderer::initRenderData() {
	// QUAD
	// positions + texture coordinates
	float vertices[] = {
		// position	// texture coordinates
		0.0f, 0.0f, 0.0f, 0.0f, // 0
		1.0f, 0.0f, 1.0f, 0.0f, // 1
		1.0f, 1.0f, 1.0f, 1.0f, // 2
		0.0f, 1.0f, 0.0f, 1.0f  // 3
	};

	// utilize indices to reuse vertices
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	// add positions + texture coordinates
	layout.Push<float>(4);
	m_VAO->AddBuffer(*m_VBO, layout);

	m_IBO = std::make_unique<IndexBuffer>(indices, 6);
};

void SpriteRenderer::DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size /* = glm::vec2(10.0f, 10.0f) */, float rotate /* = 0.0f */, glm::vec3 color /* = glm::vec3(1.0f) */) {
	Renderer renderer;

	texture->Bind();
	m_Shader->Bind();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_Shader->SetUniformMat4f("u_Model", model);
	m_Shader->SetUniformVec3f("u_SpriteColor", color);

	renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
}