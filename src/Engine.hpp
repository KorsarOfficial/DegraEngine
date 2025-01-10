#pragma once

#include "Window.hpp"
#include "Renderer/Renderer.hpp"
#include "Audio/AudioSystem.hpp"

class Engine {
public:
    Engine();
    ~Engine();

    bool Initialize();
    void Run();
    void Shutdown();

private:
    Window* m_Window;
    Renderer* m_Renderer;
    AudioSystem* m_AudioSystem;
    
    bool m_IsRunning;
}; 