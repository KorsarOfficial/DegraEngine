#pragma once

#include "Renderer.hpp"
#include <GL/glew.h>

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer() override;

    bool Initialize(Window* window) override;
    void BeginFrame() override;
    void EndFrame() override;
    void Shutdown() override;

    // Методы для рендеринга
    void DrawSprite(const std::string& texturePath, float x, float y, float width, float height);
    void DrawLine(float x1, float y1, float x2, float y2, float thickness);
    void SetClearColor(float r, float g, float b, float a);

private:
    GLuint m_ShaderProgram;
    GLuint m_VAO;
    GLuint m_VBO;
}; 