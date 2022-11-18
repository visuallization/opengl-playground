#pragma once

#include "glm/glm.hpp"

#include "Scene.h"

#include "Shader.h"
#include "Texture.h"

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace scene {
    class SceneComputeDemo : public Scene {
    public:
        SceneComputeDemo(GLFWwindow*& window);
        ~SceneComputeDemo() override;

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        glm::mat4 m_Projection;
        glm::mat4 m_View;

        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;
    };
}
