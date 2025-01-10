#include "Engine.hpp"
#include "Renderer/OpenGLRenderer.hpp"
#include "Renderer/SDLRenderer.hpp"

Engine::Engine() 
    : m_Window(nullptr)
    , m_Renderer(nullptr)
    , m_AudioSystem(nullptr)
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

    // Инициализируем аудиосистему
    m_AudioSystem = new AudioSystem();
    if (!m_AudioSystem->Initialize()) {
        return false;
    }

    m_IsRunning = true;
    return true;
}

void Engine::Run() {
    while (m_IsRunning && !m_Window->ShouldClose()) {
        // Обновление
        m_Window->Update();
        m_AudioSystem->Update();

        // Рендеринг
        m_Renderer->BeginFrame();
        // Здесь будет игровая логика
        m_Renderer->EndFrame();
    }
}

void Engine::Shutdown() {
    m_IsRunning = false;

    if (m_AudioSystem) {
        m_AudioSystem->Shutdown();
        delete m_AudioSystem;
        m_AudioSystem = nullptr;
    }

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