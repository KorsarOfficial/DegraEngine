#pragma once

#include <SDL_ttf.h>
#include <string>
#include <memory>
#include "Color.hpp"

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    // Запрещаем копирование и присваивание
    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    TextRenderer(TextRenderer&&) = delete;
    TextRenderer& operator=(TextRenderer&&) = delete;

    bool Initialize(int fontSize = 16);
    void RenderText(SDL_Renderer* renderer, const std::string& text, int x, int y, const Color& color);
    void RenderUTF8Text(SDL_Renderer* renderer, const std::string& text, int x, int y, const Color& color);
    void GetTextSize(const std::string& text, int& width, int& height);
    void GetUTF8TextSize(const std::string& text, int& width, int& height);

private:
    TTF_Font* m_Font;  // Сырой указатель, так как TTF_Font управляется SDL_ttf
}; 