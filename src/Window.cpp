#include "Window.hpp"
#include <stdexcept>

Window::Window() : m_Window(nullptr), m_Width(0), m_Height(0) {}

Window::~Window() {
    Shutdown();
}

bool Window::Initialize(const std::string& title, int width, int height) {
    if (!glfwInit()) {
        return false;
    }

    m_Title = title;
    m_Width = width;
    m_Height = height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_Window);
    return true;
}

void Window::Update() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void Window::Shutdown() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }
    glfwTerminate();
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
} 