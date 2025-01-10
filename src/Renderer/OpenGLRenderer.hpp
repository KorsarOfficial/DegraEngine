#pragma once

#include "Renderer.hpp"
#include "TextRenderer.hpp"
#include <GL/glew.h>
#include <string>
#include <memory>

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
    void DrawLine(float x1, float y1, float x2, float x2, float thickness);
    void SetClearColor(float r, float g, float b, float a);

    // Методы для текстового рендеринга
    void SetGlyph(int x, int y, const Glyph& glyph);
    void SetChar(int x, int y, char c, const Color& fg = Color(), const Color& bg = Color(0, 0, 0));
    void SetString(int x, int y, const std::string& text, const Color& fg = Color(), const Color& bg = Color(0, 0, 0));
    void ClearText(const Color& bg = Color(0, 0, 0));

private:
    GLuint m_ShaderProgram;
    GLuint m_VAO;
    GLuint m_VBO;
    
    // Шейдерная программа для текста
    GLuint m_TextShaderProgram;
    GLuint m_TextVAO;
    GLuint m_TextVBO;
    GLuint m_FontTexture;

    std::unique_ptr<TextRenderer> m_TextRenderer;
    
    // Размеры текстового буфера
    static const int TEXT_BUFFER_WIDTH = 80;
    static const int TEXT_BUFFER_HEIGHT = 25;

    // Методы для инициализации текстового рендеринга
    bool InitializeTextRendering();
    void RenderText();
    bool CreateFontTexture();
}; 