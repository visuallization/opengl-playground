#include "SceneForestFire.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

namespace scene {
    enum BoundaryCondtion {
        ASSIGNED,
        PERIODIC
    };

    enum NeighborhoodCondition
    {
        NEUMANN,
        MOORE
    };

    SceneForestFire::SceneForestFire(GLFWwindow*& window)
        : Scene::Scene(window)
        , m_SwitchTexture(false)
		, m_BoundaryCondition(BoundaryCondtion::ASSIGNED)
        , m_NeighborhoodCondition(NeighborhoodCondition::NEUMANN)
        , m_FireProbability(0.0f)
        , m_GrowthProbability(0.01f)
        , m_IsMousePressed(false)
        , m_MousePosition(-1, -1)
        , m_TreeColor(0.3f, 0.17f, 0.35f)
        , m_FireColor(0.95f, 0.46f, 0.46f)
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

        m_Shader = std::make_unique<Shader>("assets/shaders/Simple.shader");

        m_ComputeShader = std::make_unique<Shader>("assets/shaders/ForestFire.shader");

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
            // inverse y because:
            // glfwGetCursor (0,0) is at the top left corner, 
            // image2d in the shader (0,0) is at the bottom left corner
            m_MousePosition.y = m_Height - yPos;
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

        m_ComputeShader->SetUniform1i("u_BoundaryCondition", m_BoundaryCondition);
        m_ComputeShader->SetUniform1i("u_NeighborhoodCondition", m_NeighborhoodCondition);

        m_ComputeShader->SetUniform1f("u_FireProbability", m_FireProbability);
        m_ComputeShader->SetUniform1f("u_GrowthProbability", m_GrowthProbability);
		m_ComputeShader->SetUniform1f("u_Time", glfwGetTime());

        m_ComputeShader->SetUniformVec3f("u_TreeColor", m_TreeColor);
        m_ComputeShader->SetUniformVec3f("u_FireColor", m_FireColor);

		m_ComputeShader->SetUniformVec2i("u_MousePosition", m_MousePosition);

        m_ComputeShader->Dispatch(m_Width, m_Height);
        // make sure writing is completely finished
        m_ComputeShader->SetMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        m_ComputeShader->Unbind();

        m_Shader->Bind();
        m_Shader->SetUniform1i("u_Texture", m_SwitchTexture);
        renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        m_Shader->Unbind();

        m_SwitchTexture = !m_SwitchTexture;
    }

    void SceneForestFire::OnImGuiRender() {
		ImGui::Text("Neighborhood Condition: "); ImGui::SameLine();
		if (ImGui::RadioButton("Von Neumann", m_NeighborhoodCondition == NeighborhoodCondition::NEUMANN)) { m_NeighborhoodCondition = NeighborhoodCondition::NEUMANN; } ImGui::SameLine();
		if (ImGui::RadioButton("Moore", m_NeighborhoodCondition == NeighborhoodCondition::MOORE)) { m_NeighborhoodCondition = NeighborhoodCondition::MOORE; }

        ImGui::Text("Boundary Condition: "); ImGui::SameLine();
        if (ImGui::RadioButton("Assigned", m_BoundaryCondition == BoundaryCondtion::ASSIGNED)) { m_BoundaryCondition = BoundaryCondtion::ASSIGNED; } ImGui::SameLine();
        if (ImGui::RadioButton("Periodic", m_BoundaryCondition == BoundaryCondtion::PERIODIC)) { m_BoundaryCondition = BoundaryCondtion::PERIODIC; }

        ImGui::SliderFloat("Fire Probability", &m_FireProbability, 0, 1);
        ImGui::SliderFloat("Growth Probability", &m_GrowthProbability, 0, 1);

		ImGui::ColorEdit3("Tree Color", &m_TreeColor[0]);
		ImGui::ColorEdit3("Fire Color", &m_FireColor[0]);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Mouse Pos: x: %d y: %d", m_MousePosition.x, m_MousePosition.y);
    }
}