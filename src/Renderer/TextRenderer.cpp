#include "TextRenderer.hpp"

TextRenderer::TextRenderer(int width, int height)
    : m_Width(width)
    , m_Height(height)
{
    // Инициализируем буфер
    m_Buffer.resize(height, std::vector<Glyph>(width));
    Clear();
}

TextRenderer::~TextRenderer() {
}

void TextRenderer::SetGlyph(int x, int y, const Glyph& glyph) {
    if (!IsValidPosition(x, y)) return;
    m_Buffer[y][x] = glyph;
}

void TextRenderer::SetChar(int x, int y, char c, const Color& fg, const Color& bg) {
    SetGlyph(x, y, Glyph(c, fg, bg));
}

void TextRenderer::SetString(int x, int y, const std::string& text, const Color& fg, const Color& bg) {
    for (size_t i = 0; i < text.length(); ++i) {
        if (x + i >= m_Width) break;
        SetChar(x + i, y, text[i], fg, bg);
    }
}

void TextRenderer::Clear(const Color& bg) {
    Glyph empty(' ', Color(), bg);
    for (int y = 0; y < m_Height; ++y) {
        for (int x = 0; x < m_Width; ++x) {
            m_Buffer[y][x] = empty;
        }
    }
}

const Glyph& TextRenderer::GetGlyph(int x, int y) const {
    static const Glyph defaultGlyph;
    if (!IsValidPosition(x, y)) return defaultGlyph;
    return m_Buffer[y][x];
}

bool TextRenderer::IsValidPosition(int x, int y) const {
    return x >= 0 && x < m_Width && y >= 0 && y < m_Height;
} 