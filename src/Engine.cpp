#include "Engine.hpp"
#include "Renderer/SDLRenderer.hpp"
#include "Renderer/OpenGLRenderer.hpp"
#include <stdexcept>
#include <iostream>

Engine::Engine() : m_Running(false), m_Renderer(nullptr), m_Menu(nullptr) {
    std::cout << "Engine constructor called" << std::endl;
}

Engine::~Engine() {
    std::cout << "Engine destructor called" << std::endl;
    Shutdown();
}

bool Engine::Initialize(int width, int height, bool useOpenGL) {
    std::cout << "Engine::Initialize called with width=" << width << ", height=" << height << std::endl;
    
    try {
        if (m_Running) {
            std::cerr << "Engine already initialized" << std::endl;
            return false;
        }

        std::cout << "Creating renderer..." << std::endl;
        if (useOpenGL) {
            m_Renderer = std::make_unique<OpenGLRenderer>();
        } else {
            m_Renderer = std::make_unique<SDLRenderer>();
        }

        std::cout << "Initializing renderer..." << std::endl;
        m_Renderer->Initialize(width, height);

        std::cout << "Creating menu..." << std::endl;
        m_Menu = std::make_unique<Menu>(m_Renderer.get());
        std::cout << "Menu created successfully" << std::endl;

        m_Running = true;
        std::cout << "Engine initialized successfully" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка инициализации: " << e.what() << std::endl;
        Shutdown();
        return false;
    }
}

void Engine::Shutdown() {
    std::cout << "Engine::Shutdown called" << std::endl;
    
    m_Running = false;

    if (m_Menu) {
        std::cout << "Destroying menu..." << std::endl;
        m_Menu.reset();
        std::cout << "Menu destroyed" << std::endl;
    }

    if (m_Renderer) {
        std::cout << "Destroying renderer..." << std::endl;
        m_Renderer.reset();
        std::cout << "Renderer destroyed" << std::endl;
    }

    std::cout << "Engine shutdown complete" << std::endl;
}

void Engine::Run() {
    if (!m_Running || !m_Renderer || !m_Menu) {
        std::cerr << "Engine not properly initialized" << std::endl;
        return;
    }

    std::cout << "Starting main loop..." << std::endl;
    while (m_Running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    std::cout << "Quit event received" << std::endl;
                    m_Running = false;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        std::cout << "Escape key pressed" << std::endl;
                        m_Running = false;
                    }
                    break;
            }
        }

        if (!m_Running) break;

        m_Menu->Update();
        m_Renderer->Clear(Color(0, 0, 0));
        m_Menu->Render();
        m_Renderer->Present();

        SDL_Delay(16);
    }
    std::cout << "Main loop finished" << std::endl;
}

void Engine::RenderText(const std::string& text, int x, int y, const Color& color) {
    if (m_Renderer) {
        m_Renderer->RenderUTF8Text(text, x, y, color);
    } else {
        std::cerr << "Cannot render text: renderer is null" << std::endl;
    }
} 