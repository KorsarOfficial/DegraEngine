#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Color.hpp"
#include "TextRenderer.hpp"
#include "IRenderer.hpp"

class OpenGLRenderer : public IRenderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer() override;

    void Initialize(int width, int height) override;
    void Clear(const Color& color) override;
    void Present() override;

    void DrawRect(int x, int y, int width, int height, const Color& color) override;
    void FillRect(int x, int y, int width, int height, const Color& color) override;

    // Методы для работы с текстом
    void RenderText(const std::string& text, int x, int y, const Color& color) override;
    void RenderUTF8Text(const std::string& text, int x, int y, const Color& color) override;
    void GetTextSize(const std::string& text, int& width, int& height) override;
    void GetUTF8TextSize(const std::string& text, int& width, int& height) override;

private:
    GLFWwindow* m_Window;
    std::unique_ptr<TextRenderer> m_TextRenderer;
}; 