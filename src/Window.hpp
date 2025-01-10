#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    ~Window();

    bool Initialize(const std::string& title, int width, int height);
    void Update();
    void SwapBuffers();
    void Shutdown();

    bool ShouldClose() const;
    GLFWwindow* GetNativeWindow() const { return m_Window; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

private:
    GLFWwindow* m_Window;
    std::string m_Title;
    int m_Width;
    int m_Height;
}; 