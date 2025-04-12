#pragma once

#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "SpriteRenderer.h"

class PostProcessing {
    public:
        PostProcessing(int width, int height);
        ~PostProcessing();

        void Start();
        void Done();
        void Enable();
        void Disable();
        void SetShader(Shader* shader);
        void SetShader(std::string shaderName);

    private:
        // final framebuffer
        std::unique_ptr<FrameBuffer> m_FBO;
        // multisampled framebuffer
        std::unique_ptr<FrameBuffer> m_MSFBO;
        // multisampled renderbuffer
        std::unique_ptr<RenderBuffer> m_RBO;

        SpriteRenderer* m_SpriteRenderer;

        int m_Width;
        int m_Height;

        bool m_Active;

        void Draw();
        glm::mat4 GetProjectionMatrix();
};