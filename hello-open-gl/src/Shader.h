#pragma once

#include <string>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	unsigned int m_RendererID;
	ShaderProgramSource m_Source;

	ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind();
	void Unbind();

	void Set4f(const char* uniform, float x, float y, float z, float w);
};

