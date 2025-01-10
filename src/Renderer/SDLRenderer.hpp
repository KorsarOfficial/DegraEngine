#pragma once

#include <SDL.h>
#include <memory>
#include "Color.hpp"
#include "TextRenderer.hpp"
#include "IRenderer.hpp"

class SDLRenderer : public IRenderer {
public:
    SDLRenderer();
    ~SDLRenderer() override;

    // Запрещаем копирование и присваивание
    SDLRenderer(const SDLRenderer&) = delete;
    SDLRenderer& operator=(const SDLRenderer&) = delete;
    SDLRenderer(SDLRenderer&&) = delete;
    SDLRenderer& operator=(SDLRenderer&&) = delete;

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

    SDL_Renderer* GetSDLRenderer() { return m_Renderer; }

private:
    void Shutdown();

    std::unique_ptr<TextRenderer> m_TextRenderer;  // Должен быть уничтожен первым
    SDL_Renderer* m_Renderer;  // Затем этот
    SDL_Window* m_Window;  // И последним этот
}; 