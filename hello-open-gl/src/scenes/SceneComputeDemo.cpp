#include "SceneComputeDemo.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace scene {
    SceneComputeDemo::SceneComputeDemo(GLFWwindow*& window)
        : Scene::Scene(window)
    {
        // positios + texture coordinates
        float positions[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, // 0
			 1.0f, -1.0f, 1.0f, 0.0f, // 1
			 1.0f,  1.0f, 1.0f, 1.0f, // 2
			-1.0f,  1.0f, 0.0f, 1.0f  // 3
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

        m_Shader = std::make_unique<Shader>("assets/shaders/Simple.shader");

        m_ComputeShader = std::make_unique<Shader>("assets/shaders/Compute.shader");

        m_Texture = std::make_unique<Texture>(m_Width, m_Height);
    }

    SceneComputeDemo::~SceneComputeDemo() {

    }

    void SceneComputeDemo::OnUpdate(float deltaTime) {

    }

    void SceneComputeDemo::OnRender() {
        Renderer renderer;
        m_Texture->Bind();

        m_ComputeShader->Bind();
        GLCall(glDispatchCompute(m_Width, m_Height, 1));
        GLCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));
        m_ComputeShader->Unbind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Shader->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        m_Shader->Unbind();
    }

    void SceneComputeDemo::OnImGuiRender() {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}