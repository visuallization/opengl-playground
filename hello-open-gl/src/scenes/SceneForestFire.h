#pragma once

#include "Scene.h"

#include "Shader.h"
#include "Texture.h"

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace scene {
    class SceneForestFire : public Scene {
    public:
        SceneForestFire(GLFWwindow*& window);
        ~SceneForestFire() override;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;

        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Shader> m_ComputeShader;

        std::unique_ptr<Texture> m_Texture1;
        std::unique_ptr<Texture> m_Texture2;

        bool m_SwitchTexture;

        float m_FireProbability;
        float m_GrowthProbability;

		int m_BoundaryCondition;
        int m_NeighborhoodCondition;

        bool m_IsMousePressed;
        glm::ivec2 m_MousePosition;

        glm::vec3 m_TreeColor;
        glm::vec3 m_FireColor;
    };
}
