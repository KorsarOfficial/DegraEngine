#pragma once

#include <string>
#include <vector>
#include <cstdint>

// Структура для цвета
struct Color {
    uint8_t r, g, b, a;
    Color(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255)
        : r(r), g(g), b(b), a(a) {}
};

// Структура для символа с атрибутами
struct Glyph {
    char character;
    Color foreground;
    Color background;
    bool bold;
    bool italic;
    
    Glyph(char c = ' ', const Color& fg = Color(), const Color& bg = Color(0, 0, 0), bool b = false, bool i = false)
        : character(c), foreground(fg), background(bg), bold(b), italic(i) {}
};

class TextRenderer {
public:
    TextRenderer(int width, int height);
    ~TextRenderer();

    // Методы для работы с текстом
    void SetGlyph(int x, int y, const Glyph& glyph);
    void SetChar(int x, int y, char c, const Color& fg = Color(), const Color& bg = Color(0, 0, 0));
    void SetString(int x, int y, const std::string& text, const Color& fg = Color(), const Color& bg = Color(0, 0, 0));
    void Clear(const Color& bg = Color(0, 0, 0));

    // Геттеры
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    const Glyph& GetGlyph(int x, int y) const;

private:
    int m_Width;
    int m_Height;
    std::vector<std::vector<Glyph>> m_Buffer;

    bool IsValidPosition(int x, int y) const;
}; 