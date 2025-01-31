#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

struct RenderData {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};

class SpriteRenderer {
public:
	SpriteRenderer(glm::mat4 projection);
	SpriteRenderer(Shader* shader, Shader* debugShader);
	~SpriteRenderer();

	void DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec4 color = glm::vec4(1.0f));
	void DrawRectangle(glm::vec2 position, glm::vec2 size, float rotate = 0.0f);
	void DrawCircle(glm::vec2 position, glm::vec2 size);

private:
	Shader* m_Shader;
	Shader* m_DebugShader;

	std::unique_ptr<IndexBuffer> m_IBO;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;

	std::unique_ptr<IndexBuffer> m_CircleIBO;
	std::unique_ptr<VertexArray> m_CircleVAO;
	std::unique_ptr<VertexBuffer> m_CircleVBO;

	void initRenderData();

	RenderData buildCircle(float radius, unsigned int vertexCount = 8);
};

