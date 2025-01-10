#pragma once

#include "Window.hpp"
#include "Renderer/Renderer.hpp"

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
    bool m_IsRunning;
}; 