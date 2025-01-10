#pragma once

#include "../Window.hpp"
#include "TextRenderer.hpp"

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual bool Initialize(Window* window) = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Shutdown() = 0;

    // Методы для рендеринга
    virtual void DrawSprite(const std::string& texturePath, float x, float y, float width, float height) = 0;
    virtual void DrawLine(float x1, float y1, float x2, float y2, float thickness) = 0;
    virtual void SetClearColor(float r, float g, float b, float a) = 0;

    // Методы для работы с текстом
    virtual void SetGlyph(int x, int y, const Glyph& glyph) = 0;
    virtual void SetChar(int x, int y, char c, const Color& fg = Color(), const Color& bg = Color(0, 0, 0)) = 0;
    virtual void SetString(int x, int y, const std::string& text, const Color& fg = Color(), const Color& bg = Color(0, 0, 0)) = 0;
    virtual void ClearText(const Color& bg = Color(0, 0, 0)) = 0;

protected:
    Window* m_Window;
}; 