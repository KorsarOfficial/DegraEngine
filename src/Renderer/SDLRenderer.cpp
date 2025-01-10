#include "SDLRenderer.hpp"
#include <stdexcept>
#include <iostream>

#ifdef _DEBUG
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #define new DEBUG_NEW
#endif

SDLRenderer::SDLRenderer() : m_TextRenderer(nullptr), m_Renderer(nullptr), m_Window(nullptr) {
    std::cout << "SDLRenderer constructor called" << std::endl;
}

SDLRenderer::~SDLRenderer() {
    std::cout << "SDLRenderer destructor called" << std::endl;
    Shutdown();
}

void SDLRenderer::Initialize(int width, int height) {
    std::cout << "SDLRenderer::Initialize called with width=" << width << ", height=" << height << std::endl;
    
    if (m_Window || m_Renderer || m_TextRenderer) {
        std::cerr << "SDLRenderer already initialized" << std::endl;
        return;
    }

    try {
        m_Window = SDL_CreateWindow("Degra Engine",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_SHOWN);

        if (!m_Window) {
            throw std::runtime_error(std::string("Failed to create window: ") + SDL_GetError());
        }
        std::cout << "SDL Window created successfully" << std::endl;

        m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!m_Renderer) {
            throw std::runtime_error(std::string("Failed to create renderer: ") + SDL_GetError());
        }
        std::cout << "SDL Renderer created successfully" << std::endl;

        std::cout << "Creating TextRenderer..." << std::endl;
        m_TextRenderer = std::make_unique<TextRenderer>();
        if (!m_TextRenderer->Initialize(16)) {
            throw std::runtime_error("Failed to initialize text renderer");
        }
        std::cout << "TextRenderer initialized successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to initialize: " << e.what() << std::endl;
        Shutdown();
        throw;
    }
}

void SDLRenderer::Clear(const Color& color) {
    if (!m_Renderer) {
        std::cerr << "Cannot clear: renderer is null" << std::endl;
        return;
    }
    SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_Renderer);
}

void SDLRenderer::Present() {
    if (!m_Renderer) {
        std::cerr << "Cannot present: renderer is null" << std::endl;
        return;
    }
    SDL_RenderPresent(m_Renderer);
}

void SDLRenderer::DrawRect(int x, int y, int width, int height, const Color& color) {
    if (!m_Renderer) {
        std::cerr << "Cannot draw rect: renderer is null" << std::endl;
        return;
    }
    SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderDrawRect(m_Renderer, &rect);
}

void SDLRenderer::FillRect(int x, int y, int width, int height, const Color& color) {
    if (!m_Renderer) {
        std::cerr << "Cannot fill rect: renderer is null" << std::endl;
        return;
    }
    SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(m_Renderer, &rect);
}

void SDLRenderer::RenderText(const std::string& text, int x, int y, const Color& color) {
    if (!m_TextRenderer) {
        std::cerr << "Cannot render text: TextRenderer is null" << std::endl;
        return;
    }
    m_TextRenderer->RenderText(m_Renderer, text, x, y, color);
}

void SDLRenderer::RenderUTF8Text(const std::string& text, int x, int y, const Color& color) {
    if (!m_TextRenderer) {
        std::cerr << "Cannot render UTF8 text: TextRenderer is null" << std::endl;
        return;
    }
    m_TextRenderer->RenderUTF8Text(m_Renderer, text, x, y, color);
}

void SDLRenderer::GetTextSize(const std::string& text, int& width, int& height) {
    if (!m_TextRenderer) {
        std::cerr << "Cannot get text size: TextRenderer is null" << std::endl;
        width = 0;
        height = 0;
        return;
    }
    m_TextRenderer->GetTextSize(text, width, height);
}

void SDLRenderer::GetUTF8TextSize(const std::string& text, int& width, int& height) {
    if (!m_TextRenderer) {
        std::cerr << "Cannot get UTF8 text size: TextRenderer is null" << std::endl;
        width = 0;
        height = 0;
        return;
    }
    m_TextRenderer->GetUTF8TextSize(text, width, height);
}

void SDLRenderer::Shutdown() {
    std::cout << "SDLRenderer::Shutdown called" << std::endl;
    
    if (m_TextRenderer) {
        std::cout << "Destroying TextRenderer..." << std::endl;
        m_TextRenderer.reset();
        m_TextRenderer = nullptr;
        std::cout << "TextRenderer destroyed" << std::endl;
    }
    
    if (m_Renderer) {
        std::cout << "Destroying SDL Renderer..." << std::endl;
        SDL_DestroyRenderer(m_Renderer);
        m_Renderer = nullptr;
        std::cout << "SDL Renderer destroyed" << std::endl;
    }
    
    if (m_Window) {
        std::cout << "Destroying SDL Window..." << std::endl;
        SDL_DestroyWindow(m_Window);
        m_Window = nullptr;
        std::cout << "SDL Window destroyed" << std::endl;
    }
    
    std::cout << "SDLRenderer shutdown complete" << std::endl;
} 