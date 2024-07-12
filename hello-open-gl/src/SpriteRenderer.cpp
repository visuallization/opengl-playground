#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "VertexBufferLayout.h"

SpriteRenderer::SpriteRenderer(Shader* shader, Shader* debugShader) {
	// Handle transparent textures
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));
	
	// Stencil testing
	GLCall(glEnable(GL_STENCIL_TEST));
	GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
	GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));

	m_Shader = shader;
	m_DebugShader = debugShader;

	initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
	// Disable blending
	GLCall(glDisable(GL_BLEND));
	// Disable stencil testing
	GLCall(glDisable(GL_STENCIL_TEST))
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

	// CIRCLE
	RenderData circle = buildCircle(0.5f, 24);

	m_CircleVAO = std::make_unique<VertexArray>();
	m_CircleVBO = std::make_unique<VertexBuffer>(circle.vertices.data(), circle.vertices.size() * sizeof(float));

	VertexBufferLayout circleLayout;
	// add positions
	circleLayout.Push<float>(2);
	m_CircleVAO->AddBuffer(*m_CircleVBO, circleLayout);

	m_CircleIBO = std::make_unique<IndexBuffer>(circle.indices.data(), circle.indices.size());
};

RenderData SpriteRenderer::buildCircle(float radius, unsigned int vertexCount /* = 8*/) {
	RenderData data;

	float angle = 360.f / vertexCount;
	unsigned int triangleCount = vertexCount - 2;

	for (int i = 0; i < vertexCount; i++) {
		float currentAngle = angle * i;
		float x = radius * cos(glm::radians(currentAngle));
		float y = radius * sin(glm::radians(currentAngle));

		data.vertices.push_back(x);
		data.vertices.push_back(y);
	}

	for (int i = 0; i < triangleCount; i++) {
		data.indices.push_back(0);
		data.indices.push_back(i + 1);
		data.indices.push_back(i + 2);
	}

	return data;
}

void SpriteRenderer::DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size /* = glm::vec2(10.0f, 10.0f) */, float rotate /* = 0.0f */, glm::vec4 color /* = glm::vec3(1.0f) */) {
	Renderer renderer;

	texture->Bind();
	m_Shader->Bind();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	// Rotate (move pos to center, rotate, move pos back to original pos
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_Shader->SetUniformMat4f("u_Model", model);
	m_Shader->SetUniformVec4f("u_SpriteColor", color);

	renderer.Draw(*m_VAO, *m_IBO, *m_Shader);

	m_Shader->Unbind();
	texture->Unbind();
}

void SpriteRenderer::DrawRectangle(glm::vec2 position, glm::vec2 size, float rotate /* = 0.0f */) {
	// UPDATE STENCIL BUFFER
	GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
	GLCall(glStencilMask(0xFF));

	Renderer renderer;

	m_DebugShader->Bind();

	glm::mat4 model = glm::mat4(1.0f);

	// Translate
	model = glm::translate(model, glm::vec3(position, 0.0f));
	float scale = 0.9f;
	// Rotate
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	// Scale
	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_DebugShader->SetUniformMat4f("u_Model", model);
	m_DebugShader->SetUniformVec4f("u_Color", glm::vec4(1.0, 0.0, 0.0, 0.0));

	renderer.Draw(*m_VAO, *m_IBO, *m_DebugShader);

	// DRAW BORDER
	GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
	GLCall(glStencilMask(0x00));

	model = glm::mat4(1.0f);

	// Translate
	model = glm::translate(model, glm::vec3(position, 0.0f));
	// Rotate
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	// Scale
	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_DebugShader->SetUniformMat4f("u_Model", model);
	m_DebugShader->SetUniformVec4f("u_Color", glm::vec4(1.0, 0.0, 0.0, 1.0));

	renderer.Draw(*m_VAO, *m_IBO, *m_DebugShader);

	m_DebugShader->Unbind();

	GLCall(glStencilMask(0xFF));
	GLCall(glStencilFunc(GL_ALWAYS, 0, 0xFF));
}

void SpriteRenderer::DrawCircle(glm::vec2 position, glm::vec2 size) {
	// UPDATE STENCIL BUFFER
	GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
	GLCall(glStencilMask(0xFF));

	Renderer renderer;

	m_DebugShader->Bind();

	glm::mat4 model = glm::mat4(1.0f);

	// Translate
	model = glm::translate(model, glm::vec3(position, 0.0f));
	// Center
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	// Scale
	float scale = 0.8f;
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_DebugShader->SetUniformMat4f("u_Model", model);
	m_DebugShader->SetUniformVec4f("u_Color", glm::vec4(1.0, 0.0, 0.0, 0.0));

	renderer.Draw(*m_CircleVAO, *m_CircleIBO, *m_DebugShader);

	// DRAW BORDER
	GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
	GLCall(glStencilMask(0x00));

	model = glm::mat4(1.0f);

	// Translate
	model = glm::translate(model, glm::vec3(position, 0.0f));
	// Center
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	// Scale
	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_DebugShader->SetUniformMat4f("u_Model", model);
	m_DebugShader->SetUniformVec4f("u_Color", glm::vec4(1.0, 0.0, 0.0, 1.0));

	renderer.Draw(*m_CircleVAO, *m_CircleIBO, *m_DebugShader);

	m_DebugShader->Unbind();

	GLCall(glStencilMask(0xFF));
	GLCall(glStencilFunc(GL_ALWAYS, 0, 0xFF));
}
