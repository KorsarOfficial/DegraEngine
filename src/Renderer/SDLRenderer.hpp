#pragma once

#include "Renderer.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

class SDLRenderer : public Renderer {
public:
    SDLRenderer();
    ~SDLRenderer() override;

    bool Initialize(Window* window) override;
    void BeginFrame() override;
    void EndFrame() override;
    void Shutdown() override;

    // Методы для рендеринга
    void DrawSprite(const std::string& texturePath, float x, float y, float width, float height);
    void DrawLine(float x1, float y1, float x2, float y2, float thickness);
    void SetClearColor(float r, float g, float b, float a);

private:
    SDL_Renderer* m_Renderer;
    std::unordered_map<std::string, SDL_Texture*> m_TextureCache;

    SDL_Texture* LoadTexture(const std::string& path);
    void ClearTextureCache();
}; 