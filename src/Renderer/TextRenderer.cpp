#include "TextRenderer.hpp"
#include <iostream>
#include <filesystem>

#ifdef _DEBUG
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #define new DEBUG_NEW
#endif

TextRenderer::TextRenderer() : m_Font(nullptr) {
    std::cout << "TextRenderer constructor called" << std::endl;
    
    if (!TTF_WasInit()) {
        std::cerr << "WARNING: TTF was not initialized before TextRenderer creation!" << std::endl;
        throw std::runtime_error("TTF must be initialized before creating TextRenderer");
    }
}

TextRenderer::~TextRenderer() {
    std::cout << "TextRenderer destructor called" << std::endl;
    
    if (m_Font) {
        std::cout << "Closing font..." << std::endl;
        TTF_CloseFont(m_Font);
        m_Font = nullptr;
        std::cout << "Font closed successfully" << std::endl;
    }
}

bool TextRenderer::Initialize(int fontSize) {
    std::cout << "TextRenderer::Initialize called with fontSize=" << fontSize << std::endl;
    
    // Закрываем предыдущий шрифт, если он был открыт
    if (m_Font) {
        std::cout << "Closing existing font before loading new one..." << std::endl;
        TTF_CloseFont(m_Font);
        m_Font = nullptr;
    }
    
    std::string fontPath = "./assets/fonts/DejaVuSans.ttf";
    std::cout << "Attempting to load font from: " << fontPath << std::endl;
    
    if (!std::filesystem::exists(fontPath)) {
        std::cerr << "Font file does not exist at: " << std::filesystem::absolute(fontPath) << std::endl;
        
        // Попробуем найти файл в разных местах
        std::cout << "Searching for font in alternative locations..." << std::endl;
        std::vector<std::string> possiblePaths = {
            "../assets/fonts/DejaVuSans.ttf",
            "assets/fonts/DejaVuSans.ttf",
            "../../assets/fonts/DejaVuSans.ttf"
        };
        
        for (const auto& path : possiblePaths) {
            std::cout << "Trying path: " << path << std::endl;
            if (std::filesystem::exists(path)) {
                std::cout << "Found font at: " << path << std::endl;
                fontPath = path;
                break;
            }
        }
    }
    
    std::cout << "Loading font from: " << fontPath << std::endl;
    m_Font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!m_Font) {
        std::cerr << "Failed to load font from " << fontPath << ": " << TTF_GetError() << std::endl;
        return false;
    }
    
    std::cout << "Font loaded successfully!" << std::endl;
    TTF_SetFontHinting(m_Font, TTF_HINTING_LIGHT);
    return true;
}

void TextRenderer::RenderText(SDL_Renderer* renderer, const std::string& text, int x, int y, const Color& color) {
    if (!m_Font || !renderer) {
        std::cerr << "Cannot render text: " << (!m_Font ? "font not loaded" : "invalid renderer") << std::endl;
        return;
    }
    
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
    
    try {
        surface = TTF_RenderText_Blended(m_Font, text.c_str(), sdlColor);
        if (!surface) {
            throw std::runtime_error(std::string("Failed to render text surface: ") + TTF_GetError());
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            throw std::runtime_error(std::string("Failed to create texture: ") + SDL_GetError());
        }

        SDL_Rect dstRect = {x, y, surface->w, surface->h};
        SDL_FreeSurface(surface);
        surface = nullptr;

        if (SDL_RenderCopy(renderer, texture, nullptr, &dstRect) != 0) {
            throw std::runtime_error(std::string("Failed to render texture: ") + SDL_GetError());
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error rendering text: " << e.what() << std::endl;
    }

    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (surface) {
        SDL_FreeSurface(surface);
    }
}

void TextRenderer::RenderUTF8Text(SDL_Renderer* renderer, const std::string& text, int x, int y, const Color& color) {
    if (!m_Font || !renderer) {
        std::cerr << "Cannot render UTF8 text: " << (!m_Font ? "font not loaded" : "invalid renderer") << std::endl;
        return;
    }
    
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
    
    try {
        surface = TTF_RenderUTF8_Blended(m_Font, text.c_str(), sdlColor);
        if (!surface) {
            throw std::runtime_error(std::string("Failed to render UTF8 surface: ") + TTF_GetError());
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            SDL_FreeSurface(surface);
            throw std::runtime_error(std::string("Failed to create texture: ") + SDL_GetError());
        }

        SDL_Rect dstRect = {x, y, surface->w, surface->h};
        SDL_FreeSurface(surface);
        surface = nullptr;

        if (SDL_RenderCopy(renderer, texture, nullptr, &dstRect) != 0) {
            throw std::runtime_error(std::string("Failed to render texture: ") + SDL_GetError());
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error rendering UTF8 text: " << e.what() << std::endl;
    }

    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (surface) {
        SDL_FreeSurface(surface);
    }
}

void TextRenderer::GetTextSize(const std::string& text, int& width, int& height) {
    if (!m_Font) {
        std::cerr << "Cannot get text size: font not loaded" << std::endl;
        width = 0;
        height = 0;
        return;
    }
    TTF_SizeText(m_Font, text.c_str(), &width, &height);
}

void TextRenderer::GetUTF8TextSize(const std::string& text, int& width, int& height) {
    if (!m_Font) {
        std::cerr << "Cannot get UTF8 text size: font not loaded" << std::endl;
        width = 0;
        height = 0;
        return;
    }
    TTF_SizeUTF8(m_Font, text.c_str(), &width, &height);
} 