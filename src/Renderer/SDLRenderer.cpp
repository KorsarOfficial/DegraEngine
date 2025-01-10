#include "SDLRenderer.hpp"
#include <SDL_image.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// Размеры символа в пикселях
const int CHAR_WIDTH = 8;
const int CHAR_HEIGHT = 16;

SDLRenderer::SDLRenderer() 
    : m_Renderer(nullptr)
    , m_TextRenderer(std::make_unique<TextRenderer>(TEXT_BUFFER_WIDTH, TEXT_BUFFER_HEIGHT))
{}

SDLRenderer::~SDLRenderer() {
    Shutdown();
}

bool SDLRenderer::Initialize(Window* window) {
    m_Window = window;

    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    // Создаем SDL рендерер
    SDL_Window* sdlWindow = SDL_CreateWindowFrom(glfwGetWin32Window(window->GetNativeWindow()));
    if (!sdlWindow) {
        return false;
    }

    m_Renderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_Renderer) {
        return false;
    }

    // Инициализация SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        return false;
    }

    return true;
}

void SDLRenderer::BeginFrame() {
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_Renderer);
}

void SDLRenderer::EndFrame() {
    RenderText();
    SDL_RenderPresent(m_Renderer);
}

void SDLRenderer::RenderText() {
    // Для каждого символа в буфере
    for (int y = 0; y < m_TextRenderer->GetHeight(); ++y) {
        for (int x = 0; x < m_TextRenderer->GetWidth(); ++x) {
            const Glyph& glyph = m_TextRenderer->GetGlyph(x, y);

            // Рисуем фон
            SDL_Rect bgRect = {
                x * CHAR_WIDTH,
                y * CHAR_HEIGHT,
                CHAR_WIDTH,
                CHAR_HEIGHT
            };
            SDL_SetRenderDrawColor(m_Renderer, 
                glyph.background.r,
                glyph.background.g,
                glyph.background.b,
                glyph.background.a);
            SDL_RenderFillRect(m_Renderer, &bgRect);

            // Рисуем символ (в SDL2 это упрощенно, для полной реализации нужно использовать SDL_ttf)
            SDL_SetRenderDrawColor(m_Renderer,
                glyph.foreground.r,
                glyph.foreground.g,
                glyph.foreground.b,
                glyph.foreground.a);
            
            // Рисуем простой прямоугольник вместо символа (временно)
            SDL_Rect charRect = {
                x * CHAR_WIDTH + 2,
                y * CHAR_HEIGHT + 2,
                CHAR_WIDTH - 4,
                CHAR_HEIGHT - 4
            };
            if (glyph.character != ' ') {
                SDL_RenderFillRect(m_Renderer, &charRect);
            }
        }
    }
}

void SDLRenderer::SetGlyph(int x, int y, const Glyph& glyph) {
    m_TextRenderer->SetGlyph(x, y, glyph);
}

void SDLRenderer::SetChar(int x, int y, char c, const Color& fg, const Color& bg) {
    m_TextRenderer->SetChar(x, y, c, fg, bg);
}

void SDLRenderer::SetString(int x, int y, const std::string& text, const Color& fg, const Color& bg) {
    m_TextRenderer->SetString(x, y, text, fg, bg);
}

void SDLRenderer::ClearText(const Color& bg) {
    m_TextRenderer->Clear(bg);
}

void SDLRenderer::Shutdown() {
    ClearTextureCache();

    if (m_Renderer) {
        SDL_DestroyRenderer(m_Renderer);
        m_Renderer = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}

void SDLRenderer::DrawSprite(const std::string& texturePath, float x, float y, float width, float height) {
    SDL_Texture* texture = LoadTexture(texturePath);
    if (!texture) return;

    SDL_Rect dstRect;
    dstRect.x = static_cast<int>(x);
    dstRect.y = static_cast<int>(y);
    dstRect.w = static_cast<int>(width);
    dstRect.h = static_cast<int>(height);

    SDL_RenderCopy(m_Renderer, texture, nullptr, &dstRect);
}

void SDLRenderer::DrawLine(float x1, float y1, float x2, float y2, float thickness) {
    SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
    
    if (thickness <= 1.0f) {
        SDL_RenderDrawLine(m_Renderer, 
            static_cast<int>(x1), 
            static_cast<int>(y1), 
            static_cast<int>(x2), 
            static_cast<int>(y2));
    } else {
        // Для толстых линий рисуем несколько параллельных линий
        for (float i = -thickness/2; i < thickness/2; i += 1.0f) {
            SDL_RenderDrawLine(m_Renderer, 
                static_cast<int>(x1 + i), 
                static_cast<int>(y1 + i), 
                static_cast<int>(x2 + i), 
                static_cast<int>(y2 + i));
        }
    }
}

void SDLRenderer::SetClearColor(float r, float g, float b, float a) {
    SDL_SetRenderDrawColor(m_Renderer, 
        static_cast<Uint8>(r * 255), 
        static_cast<Uint8>(g * 255), 
        static_cast<Uint8>(b * 255), 
        static_cast<Uint8>(a * 255));
}

SDL_Texture* SDLRenderer::LoadTexture(const std::string& path) {
    // Проверяем кэш
    auto it = m_TextureCache.find(path);
    if (it != m_TextureCache.end()) {
        return it->second;
    }

    // Загружаем новую текстуру
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
    SDL_FreeSurface(surface);

    if (texture) {
        m_TextureCache[path] = texture;
    }

    return texture;
}

void SDLRenderer::ClearTextureCache() {
    for (auto& pair : m_TextureCache) {
        SDL_DestroyTexture(pair.second);
    }
    m_TextureCache.clear();
} 