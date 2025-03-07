#pragma once

#include "FrameBuffer.h"
#include "RenderBuffer.h"
#include "SpriteRenderer.h"

class PostProcessing {
    public:
        PostProcessing(int width, int height);
        ~PostProcessing();

        void Bind();
        void Unbind();
        void Draw();

    private:
        std::unique_ptr<FrameBuffer> m_FBO;
        std::unique_ptr<RenderBuffer> m_RBO;

        std::shared_ptr<Shader> m_Shader;
        SpriteRenderer* m_SpriteRenderer;

        int m_Width;
        int m_Height;
};