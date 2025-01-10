#include "OpenGLRenderer.hpp"
#include <stdexcept>

OpenGLRenderer::OpenGLRenderer() {}

OpenGLRenderer::~OpenGLRenderer() {
    glfwTerminate();
}

void OpenGLRenderer::Initialize(int width, int height) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    m_Window = glfwCreateWindow(width, height, "Degra Engine", nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_Window);

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_TextRenderer = std::make_unique<TextRenderer>();
    if (!m_TextRenderer->Initialize()) {
        throw std::runtime_error("Failed to initialize text renderer");
    }
}

void OpenGLRenderer::Clear(const Color& color) {
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::Present() {
    glfwSwapBuffers(m_Window);
}

void OpenGLRenderer::DrawRect(int x, int y, int width, int height, const Color& color) {
    glColor4ub(color.r, color.g, color.b, color.a);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();
}

void OpenGLRenderer::FillRect(int x, int y, int width, int height, const Color& color) {
    glColor4ub(color.r, color.g, color.b, color.a);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();
}

void OpenGLRenderer::RenderText(const std::string& text, int x, int y, const Color& color) {
    // В OpenGL рендерер мы не можем использовать SDL_Renderer
    // Поэтому здесь нужно реализовать отдельный механизм рендеринга текста
    // Это может быть сделано с помощью текстур или других методов
}

void OpenGLRenderer::RenderUTF8Text(const std::string& text, int x, int y, const Color& color) {
    RenderText(text, x, y, color);
}

void OpenGLRenderer::GetTextSize(const std::string& text, int& width, int& height) {
    m_TextRenderer->GetTextSize(text, width, height);
}

void OpenGLRenderer::GetUTF8TextSize(const std::string& text, int& width, int& height) {
    m_TextRenderer->GetUTF8TextSize(text, width, height);
} 