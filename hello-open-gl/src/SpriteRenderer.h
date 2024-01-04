#pragma once

#include <memory>

#include "glm/glm.hpp"

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class SpriteRenderer {
public:
	SpriteRenderer(Shader* shader);
	~SpriteRenderer() {}

	void DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate  = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	Shader* m_Shader;
	std::unique_ptr<IndexBuffer> m_IBO;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;

	void initRenderData();
};

