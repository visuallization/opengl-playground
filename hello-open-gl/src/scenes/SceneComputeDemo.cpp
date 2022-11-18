#include "SceneComputeDemo.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace scene {
    SceneComputeDemo::SceneComputeDemo(GLFWwindow*& window)
        : Scene::Scene(window)
        , m_Projection(glm::ortho(0.0f, (float)m_Width, 0.0f, (float)m_Height, -1.0f, 1.0f))
        , m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
    {
        // positios + texture coordinates
        float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f, // 0
			 50.0f, -50.0f, 1.0f, 0.0f, // 1
			 50.0f,  50.0f, 1.0f, 1.0f, // 2
			-50.0f,  50.0f, 0.0f, 1.0f  // 3
		};

        // utilize indices to reuse vertices
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
		};

        m_VAO = std::make_unique<VertexArray>();
        m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        // add positions
        layout.Push<float>(2);
        // add texture coordinates
        layout.Push<float>(2);

        m_VAO->AddBuffer(*m_VBO, layout);
        
        m_IBO = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_Shader->Unbind();

        m_ComputeShader = std::make_unique<Shader>("res/shaders/Compute.shader");
        m_ComputeShader->Bind();
        m_ComputeShader->Unbind();

        //m_Texture = std::make_unique<Texture>("res/textures/emoji.png");

        glGenTextures(1, &m_TextureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 512, 512, 0, GL_RGBA,
            GL_FLOAT, NULL);

        glBindImageTexture(0, m_TextureID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    }

    SceneComputeDemo::~SceneComputeDemo() {

    }

    void SceneComputeDemo::OnUpdate(float deltaTime) {

    }

    void SceneComputeDemo::OnRender() {
        Renderer renderer;
        //m_Texture->Bind();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
        glm::mat4 mvp = m_Projection * m_View * model;

        m_ComputeShader->Bind();
        glDispatchCompute((unsigned int)512, (unsigned int)512, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        m_ComputeShader->Unbind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        m_Shader->SetUniform1i("u_Texture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        m_Shader->Unbind();
    }

    void SceneComputeDemo::OnImGuiRender() {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}