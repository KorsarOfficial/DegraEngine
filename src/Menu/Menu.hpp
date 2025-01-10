#pragma once

#include "../Renderer/IRenderer.hpp"
#include <memory>

class Menu {
public:
    explicit Menu(IRenderer* renderer);
    ~Menu();

    void Update();
    void Render();

private:
    IRenderer* m_Renderer;  // Weak pointer, не владеем объектом
}; 