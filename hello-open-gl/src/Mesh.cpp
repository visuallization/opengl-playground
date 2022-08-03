#include "Mesh.h"

Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures)
	: Vertices(vertices), Indices(indices), Textures(textures)
{
	VAO = std::make_unique<VertexArray>();
	// 3 floats positions + 3 floats normals + 2 floats texture coords = 8
	VBO = std::make_unique<VertexBuffer>(&Vertices[0], Vertices.size() * 8 * sizeof(float));

	VertexBufferLayout layout;
	// add positions
	layout.Push<float>(3);
	// add normals
	layout.Push<float>(3);
	// add texture coordinates
	layout.Push<float>(2);

	VAO->AddBuffer(*VBO, layout);

	IBO = std::make_unique<IndexBuffer>(&Indices[0], Indices.size());
}