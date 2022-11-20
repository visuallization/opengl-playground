#include "SceneForestFire.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace scene {
    SceneForestFire::SceneForestFire(GLFWwindow*& window)
        : Scene::Scene(window)
    {
        // positions + texture coordinates
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

		/*m_Texture1 = std::make_unique<Texture>(m_Width, m_Height);
		m_Texture2 = std::make_unique<Texture>(m_Width, m_Height);*/

		GLCall(glGenTextures(2, m_Textures));
        for (int i = 0; i < 2; i++) {
			GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i]));

			// The texture minifying function is used whenever the level-of-detail function determines that the texture should be minified
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			// The texture magnification function is used whenever the level-of-detail function determines that the texture should be magnified
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			// Clamp texture horizontally
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			// Clamp texture vertically
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL));

			GLCall(glBindImageTexture(i, m_Textures[i], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F));
        }
	}

    SceneForestFire::~SceneForestFire() {

    }

    void SceneForestFire::OnUpdate(float deltaTime) {

    }

    void SceneForestFire::OnRender() {
        Renderer renderer;
		GLCall(glActiveTexture(GL_TEXTURE0 + 0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[1]));
		GLCall(glBindImageTexture(1, m_Textures[1], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F))
        //m_Texture1->BindImage();
        //m_Texture2->BindImage();

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

    void SceneForestFire::OnImGuiRender() {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}