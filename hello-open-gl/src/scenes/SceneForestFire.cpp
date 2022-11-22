#include "SceneForestFire.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace scene {
    SceneForestFire::SceneForestFire(GLFWwindow*& window)
        : Scene::Scene(window)
        , m_SwitchTexture(false)
        , m_FireProbability(0.0f)
        , m_GrowthProbability(0.0f)
        , m_IsMousePressed(false)
        , m_MousePosition(-1, -1)
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

		m_Texture1 = std::make_unique<Texture>(m_Width, m_Height, 0);
		m_Texture2 = std::make_unique<Texture>(m_Width, m_Height, 1);
	}

    SceneForestFire::~SceneForestFire() {

    }

    void SceneForestFire::OnUpdate(float deltaTime) {
        if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            m_IsMousePressed = true;
            double xPos, yPos;
			glfwGetCursorPos(m_Window, &xPos, &yPos);
            m_MousePosition.x = xPos;
            m_MousePosition.y = yPos;
        }
        else if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
            m_IsMousePressed = false;
            m_MousePosition.x = -1;
            m_MousePosition.y = -1;
        }
    }

    void SceneForestFire::OnRender() {
        Renderer renderer;
        m_Texture1->Bind(0);
        m_Texture2->Bind(1);

        m_ComputeShader->Bind();
        m_ComputeShader->SetUniform1i("u_SwitchTexture", m_SwitchTexture);
        m_ComputeShader->SetUniform1f("u_FireProbability", m_FireProbability);
        m_ComputeShader->SetUniform1f("u_GrowthProbability", m_GrowthProbability);
        m_ComputeShader->SetUniform1f("u_Time", glfwGetTime());
        GLCall(glDispatchCompute(m_Width, m_Height, 1));
        GLCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));
        m_ComputeShader->Unbind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_Shader->Bind();
        m_Shader->SetUniform1i("u_Texture", m_SwitchTexture);
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        m_Shader->Unbind();

        m_SwitchTexture = !m_SwitchTexture;
    }

    void SceneForestFire::OnImGuiRender() {
        //ImGui::Checkbox("Switch Texture", &m_SwitchTexture);
        ImGui::SliderFloat("Fire Probability", &m_FireProbability, 0, 1);
        ImGui::SliderFloat("Growth Probability", &m_GrowthProbability, 0, 1);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Mouse Pos: x: %d y: %d", m_MousePosition.x, m_MousePosition.y);
    }
}