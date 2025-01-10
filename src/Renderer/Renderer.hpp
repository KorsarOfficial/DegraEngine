#pragma once

#include "../Window.hpp"

class Renderer {
public:
    virtual ~Renderer() = default;

    virtual bool Initialize(Window* window) = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
    virtual void Shutdown() = 0;

protected:
    Window* m_Window;
}; 