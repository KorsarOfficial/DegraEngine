#include "Engine.hpp"
#include "Renderer/OpenGLRenderer.hpp"
#include "Renderer/SDLRenderer.hpp"

Engine::Engine() 
    : m_Window(nullptr)
    , m_Renderer(nullptr)
    , m_IsRunning(false) {}

Engine::~Engine() {
    Shutdown();
}

bool Engine::Initialize() {
    // Создаем окно
    m_Window = new Window();
    if (!m_Window->Initialize("Degra Engine", 1280, 720)) {
        return false;
    }

    // Создаем рендерер в зависимости от конфигурации
    #ifdef USE_LEGACY_RENDERER
        m_Renderer = new OpenGLRenderer();
    #else
        m_Renderer = new SDLRenderer();
    #endif

    if (!m_Renderer->Initialize(m_Window)) {
        return false;
    }

    m_IsRunning = true;
    return true;
}

void Engine::Run() {
    // Тестовый текст
    m_Renderer->SetString(0, 0, "Degra Engine - ASCII Mode", Color(255, 255, 255), Color(0, 0, 0));
    m_Renderer->SetString(0, 2, "Press ESC to exit", Color(255, 255, 0), Color(0, 0, 0));
    
    // Рамка
    for (int x = 0; x < 80; ++x) {
        m_Renderer->SetChar(x, 4, '-', Color(255, 255, 255), Color(0, 0, 0));
        m_Renderer->SetChar(x, 24, '-', Color(255, 255, 255), Color(0, 0, 0));
    }
    for (int y = 4; y <= 24; ++y) {
        m_Renderer->SetChar(0, y, '|', Color(255, 255, 255), Color(0, 0, 0));
        m_Renderer->SetChar(79, y, '|', Color(255, 255, 255), Color(0, 0, 0));
    }

    while (m_IsRunning && !m_Window->ShouldClose()) {
        // Обновление
        m_Window->Update();

        // Рендеринг
        m_Renderer->BeginFrame();
        m_Renderer->EndFrame();
    }
}

void Engine::Shutdown() {
    m_IsRunning = false;

    if (m_Renderer) {
        m_Renderer->Shutdown();
        delete m_Renderer;
        m_Renderer = nullptr;
    }

    if (m_Window) {
        m_Window->Shutdown();
        delete m_Window;
        m_Window = nullptr;
    }
} 