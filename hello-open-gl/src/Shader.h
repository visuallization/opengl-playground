#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
	std::string ComputeSource;
};

class Shader {
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms of the shader (after it has been activated by glUseProgram)
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float x, float y, float z, float w);
	void SetUniformVec2i(const std::string& name, glm::ivec2 vector);
	void SetUniformVec3i(const std::string& name, glm::ivec3 vector);
	void SetUniformVec4i(const std::string& name, glm::ivec4 vector);
	void SetUniformVec2f(const std::string& name, glm::vec2 vector);
	void SetUniformVec3f(const std::string& name, glm::vec3 vector);
	void SetUniformVec4f(const std::string& name, glm::vec4 vector);
	void SetUniformMat4f(const std::string& name, glm::mat4 matrix);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader, const std::string& computeShader);
	int GetUniformLocation(const std::string& name);
};

