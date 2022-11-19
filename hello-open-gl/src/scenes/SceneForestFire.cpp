#include "SceneForestFire.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace scene {
    SceneForestFire::SceneForestFire(GLFWwindow*& window)
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

        m_Shader = std::make_unique<Shader>("res/shaders/Simple.shader");

        m_ComputeShader = std::make_unique<Shader>("res/shaders/ForestFire.shader");

        m_Texture = std::make_unique<Texture>(m_Width, m_Height);
    }

    SceneForestFire::~SceneForestFire() {

    }

    void SceneForestFire::OnUpdate(float deltaTime) {

    }

    void SceneForestFire::OnRender() {
        Renderer renderer;

        m_ComputeShader->Bind();
        GLCall(glDispatchCompute(m_Width, m_Height, 1));
        GLCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));
        m_ComputeShader->Unbind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Shader->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);
        m_Texture->Bind();
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        m_Shader->Unbind();
    }

    void SceneForestFire::OnImGuiRender() {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}