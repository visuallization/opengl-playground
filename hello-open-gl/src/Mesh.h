#pragma once

#include <memory>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "IndexBuffer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

struct MeshVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh {
public:
	std::vector<MeshVertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<std::shared_ptr<Texture>> Textures;

	std::unique_ptr<VertexArray> VAO;
	std::unique_ptr<VertexBuffer> VBO;
	std::unique_ptr<IndexBuffer> IBO;

	Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures);
};