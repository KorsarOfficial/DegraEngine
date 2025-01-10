#pragma once

#include "Color.hpp"
#include <string>

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void Initialize(int width, int height) = 0;
    virtual void Clear(const Color& color) = 0;
    virtual void Present() = 0;

    virtual void DrawRect(int x, int y, int width, int height, const Color& color) = 0;
    virtual void FillRect(int x, int y, int width, int height, const Color& color) = 0;

    // Методы для работы с текстом
    virtual void RenderText(const std::string& text, int x, int y, const Color& color) = 0;
    virtual void RenderUTF8Text(const std::string& text, int x, int y, const Color& color) = 0;
    virtual void GetTextSize(const std::string& text, int& width, int& height) = 0;
    virtual void GetUTF8TextSize(const std::string& text, int& width, int& height) = 0;
}; 