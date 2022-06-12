#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Renderer.h"

Shader::Shader(const std::string& filepath): m_Filepath(filepath), m_RendererID(0) {
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource, source.GeometrySource);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w) {
    GLCall(glUniform4f(GetUniformLocation(name), x, y, z, w));
}

void Shader::SetUniformVec3f(const std::string& name, glm::vec3 vector) {
    GLCall(glUniform3fv(GetUniformLocation(name), 1, &vector[0]));
}

void Shader::SetUniformVec4f(const std::string& name, glm::vec4 vector) {
    GLCall(glUniform4fv(GetUniformLocation(name), 1, &vector[0]));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4 matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
        GEOMETRY = 2
    };

    std::string line;
    std::stringstream ss[3];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
            else if (line.find("geometry") != std::string::npos) {
                type = ShaderType::GEOMETRY;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str(), ss[2].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    std::string shaderType = "";
    switch (type) {
    case GL_VERTEX_SHADER:
        shaderType = "vertex";
        break;
    case GL_FRAGMENT_SHADER:
        shaderType = "fragment";
        break;
    case GL_GEOMETRY_SHADER:
        shaderType = "geometry";
        break;
    default:
        shaderType = "undefined";
    }

    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << shaderType << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader) {
    GLCall(unsigned int program = glCreateProgram());

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    unsigned int gs;

    if (geometryShader != "") {
        gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
    }

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    if (geometryShader != "") {
        GLCall(glAttachShader(program, gs));
    }

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    if (geometryShader != "") {
        GLCall(glDeleteShader(gs));
    }

    return program;
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    // get the index/id of the uniform
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    // check if the uniform exists in the shader
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;

    return location;
}