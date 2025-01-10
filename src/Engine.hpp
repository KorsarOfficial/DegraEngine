#pragma once

#include <memory>
#include "Menu/Menu.hpp"
#include "Renderer/IRenderer.hpp"

class Engine {
public:
    Engine();
    ~Engine();

    // Запрещаем копирование и присваивание
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    bool Initialize(int width, int height, bool useOpenGL = false);
    void Run();
    void Shutdown();
    
    void RenderText(const std::string& text, int x, int y, const Color& color = Color(255, 255, 255));

private:
    std::unique_ptr<IRenderer> m_Renderer;
    std::unique_ptr<Menu> m_Menu;
    bool m_Running;
}; 