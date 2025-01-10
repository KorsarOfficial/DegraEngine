#pragma once

#include "Renderer.hpp"
#include <SDL.h>
#include <string>
#include <unordered_map>
#include <memory>

class SDLRenderer : public Renderer {
public:
    SDLRenderer();
    ~SDLRenderer() override;

    bool Initialize(Window* window) override;
    void BeginFrame() override;
    void EndFrame() override;
    void Shutdown() override;

    // Методы для рендеринга
    void DrawSprite(const std::string& texturePath, float x, float y, float width, float height) override;
    void DrawLine(float x1, float y1, float x2, float x2, float thickness) override;
    void SetClearColor(float r, float g, float b, float a) override;

    // Методы для работы с текстом
    void SetGlyph(int x, int y, const Glyph& glyph) override;
    void SetChar(int x, int y, char c, const Color& fg = Color(), const Color& bg = Color(0, 0, 0)) override;
    void SetString(int x, int y, const std::string& text, const Color& fg = Color(), const Color& bg = Color(0, 0, 0)) override;
    void ClearText(const Color& bg = Color(0, 0, 0)) override;

private:
    SDL_Renderer* m_Renderer;
    std::unordered_map<std::string, SDL_Texture*> m_TextureCache;
    std::unique_ptr<TextRenderer> m_TextRenderer;

    // Размеры текстового буфера
    static const int TEXT_BUFFER_WIDTH = 80;
    static const int TEXT_BUFFER_HEIGHT = 25;

    SDL_Texture* LoadTexture(const std::string& path);
    void ClearTextureCache();
    void RenderText();
}; 