#include "OpenGLRenderer.hpp"
#include <stdexcept>

// Размеры символа в пикселях
const int CHAR_WIDTH = 8;
const int CHAR_HEIGHT = 16;

// Базовый набор ASCII символов (первые 128 символов)
const unsigned char fontData8x16[128 * 16] = {
    // Здесь данные для символа 0x00
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // Символ 'A' (0x41)
    0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00,
    // Символ 'B' (0x42)
    0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00,
    // ... добавьте остальные символы
};

OpenGLRenderer::OpenGLRenderer() 
    : m_ShaderProgram(0)
    , m_VAO(0)
    , m_VBO(0)
    , m_TextShaderProgram(0)
    , m_TextVAO(0)
    , m_TextVBO(0)
    , m_FontTexture(0)
    , m_TextRenderer(std::make_unique<TextRenderer>(TEXT_BUFFER_WIDTH, TEXT_BUFFER_HEIGHT))
{
}

OpenGLRenderer::~OpenGLRenderer() {
    Shutdown();
}

bool OpenGLRenderer::Initialize(Window* window) {
    m_Window = window;

    // Инициализация GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return false;
    }

    // Создаем и компилируем шейдеры для обычного рендеринга
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main() {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
    )";

    // Компилируем шейдеры для обычного рендеринга
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    m_ShaderProgram = glCreateProgram();
    glAttachShader(m_ShaderProgram, vertexShader);
    glAttachShader(m_ShaderProgram, fragmentShader);
    glLinkProgram(m_ShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Создаем VAO и VBO для обычного рендеринга
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    // Инициализируем текстовый рендеринг
    if (!InitializeTextRendering()) {
        return false;
    }

    return true;
}

bool OpenGLRenderer::InitializeTextRendering() {
    // Шейдеры для текстового рендеринга
    const char* textVertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
        out vec2 TexCoords;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
            TexCoords = vertex.zw;
        }
    )";

    const char* textFragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoords;
        out vec4 color;
        uniform sampler2D text;
        uniform vec4 textColor;

        void main() {
            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
            color = textColor * sampled;
        }
    )";

    // Компилируем шейдеры для текста
    GLuint textVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(textVertexShader, 1, &textVertexShaderSource, NULL);
    glCompileShader(textVertexShader);

    GLuint textFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(textFragmentShader, 1, &textFragmentShaderSource, NULL);
    glCompileShader(textFragmentShader);

    m_TextShaderProgram = glCreateProgram();
    glAttachShader(m_TextShaderProgram, textVertexShader);
    glAttachShader(m_TextShaderProgram, textFragmentShader);
    glLinkProgram(m_TextShaderProgram);

    glDeleteShader(textVertexShader);
    glDeleteShader(textFragmentShader);

    // Создаем VAO и VBO для текста
    glGenVertexArrays(1, &m_TextVAO);
    glGenBuffers(1, &m_TextVBO);
    glBindVertexArray(m_TextVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_TextVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return CreateFontTexture();
}

bool OpenGLRenderer::CreateFontTexture() {
    // Создаем текстуру
    glGenTextures(1, &m_FontTexture);
    glBindTexture(GL_TEXTURE_2D, m_FontTexture);

    // Создаем временный буфер для текстуры
    const int TEXTURE_WIDTH = CHAR_WIDTH * 16;  // 16 символов в ширину
    const int TEXTURE_HEIGHT = CHAR_HEIGHT * 8; // 8 символов в высоту
    std::vector<unsigned char> textureData(TEXTURE_WIDTH * TEXTURE_HEIGHT, 0);

    // Заполняем текстуру данными шрифта
    for (int charIndex = 0; charIndex < 128; ++charIndex) {
        int baseX = (charIndex % 16) * CHAR_WIDTH;
        int baseY = (charIndex / 16) * CHAR_HEIGHT;

        for (int y = 0; y < CHAR_HEIGHT; ++y) {
            unsigned char row = fontData8x16[charIndex * CHAR_HEIGHT + y];
            for (int x = 0; x < CHAR_WIDTH; ++x) {
                bool pixel = (row & (0x80 >> x)) != 0;
                textureData[(baseY + y) * TEXTURE_WIDTH + baseX + x] = pixel ? 255 : 0;
            }
        }
    }

    // Загружаем текстуру в OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, textureData.data());

    // Настраиваем параметры текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return true;
}

void OpenGLRenderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_ShaderProgram);
}

void OpenGLRenderer::EndFrame() {
    RenderText();
    glfwSwapBuffers(m_Window->GetNativeWindow());
}

void OpenGLRenderer::RenderText() {
    glUseProgram(m_TextShaderProgram);
    glBindVertexArray(m_TextVAO);
    glBindTexture(GL_TEXTURE_2D, m_FontTexture);

    // Включаем смешивание для прозрачности
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Для каждого символа в буфере
    for (int y = 0; y < m_TextRenderer->GetHeight(); ++y) {
        for (int x = 0; x < m_TextRenderer->GetWidth(); ++x) {
            const Glyph& glyph = m_TextRenderer->GetGlyph(x, y);
            if (glyph.character == ' ') continue;

            // Вычисляем координаты текстуры для символа
            float texX = (glyph.character % 16) / 16.0f;
            float texY = (glyph.character / 16) / 8.0f;
            float texWidth = 1.0f / 16.0f;
            float texHeight = 1.0f / 8.0f;

            // Вычисляем координаты вершин
            float xPos = x * CHAR_WIDTH;
            float yPos = y * CHAR_HEIGHT;

            // Создаем вершины для символа
            float vertices[6][4] = {
                { xPos,              yPos + CHAR_HEIGHT,  texX,          texY + texHeight },
                { xPos + CHAR_WIDTH, yPos,               texX + texWidth, texY },
                { xPos,              yPos,               texX,           texY },

                { xPos,              yPos + CHAR_HEIGHT,  texX,          texY + texHeight },
                { xPos + CHAR_WIDTH, yPos + CHAR_HEIGHT,  texX + texWidth, texY + texHeight },
                { xPos + CHAR_WIDTH, yPos,               texX + texWidth, texY }
            };

            // Обновляем VBO
            glBindBuffer(GL_ARRAY_BUFFER, m_TextVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            // Устанавливаем цвет текста
            glUniform4f(glGetUniformLocation(m_TextShaderProgram, "textColor"),
                glyph.foreground.r / 255.0f,
                glyph.foreground.g / 255.0f,
                glyph.foreground.b / 255.0f,
                glyph.foreground.a / 255.0f);

            // Рисуем символ
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    glDisable(GL_BLEND);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void OpenGLRenderer::Shutdown() {
    if (m_TextShaderProgram) {
        glDeleteProgram(m_TextShaderProgram);
        m_TextShaderProgram = 0;
    }
    if (m_TextVAO) {
        glDeleteVertexArrays(1, &m_TextVAO);
        m_TextVAO = 0;
    }
    if (m_TextVBO) {
        glDeleteBuffers(1, &m_TextVBO);
        m_TextVBO = 0;
    }
    if (m_FontTexture) {
        glDeleteTextures(1, &m_FontTexture);
        m_FontTexture = 0;
    }

    if (m_ShaderProgram) {
        glDeleteProgram(m_ShaderProgram);
        m_ShaderProgram = 0;
    }
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
}

// Методы для работы с текстом
void OpenGLRenderer::SetGlyph(int x, int y, const Glyph& glyph) {
    m_TextRenderer->SetGlyph(x, y, glyph);
}

void OpenGLRenderer::SetChar(int x, int y, char c, const Color& fg, const Color& bg) {
    m_TextRenderer->SetChar(x, y, c, fg, bg);
}

void OpenGLRenderer::SetString(int x, int y, const std::string& text, const Color& fg, const Color& bg) {
    m_TextRenderer->SetString(x, y, text, fg, bg);
}

void OpenGLRenderer::ClearText(const Color& bg) {
    m_TextRenderer->Clear(bg);
}

// Остальные методы рендеринга остаются без изменений
void OpenGLRenderer::DrawSprite(const std::string& texturePath, float x, float y, float width, float height) {
    // Вершины спрайта
    float vertices[] = {
        x,          y,           0.0f,
        x + width,  y,           0.0f,
        x + width,  y + height,  0.0f,
        x,          y + height,  0.0f
    };

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void OpenGLRenderer::DrawLine(float x1, float y1, float x2, float y2, float thickness) {
    float vertices[] = {
        x1, y1, 0.0f,
        x2, y2, 0.0f
    };

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glLineWidth(thickness);
    glDrawArrays(GL_LINES, 0, 2);
}

void OpenGLRenderer::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
} 