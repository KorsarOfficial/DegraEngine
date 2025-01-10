#include "WorldGenerator.hpp"
#include <random>
#include <chrono>

WorldGenerator::WorldGenerator(int width, int height)
    : m_Width(width)
    , m_Height(height)
    , m_Progress(0.0f)
    , m_IsGenerating(false)
{
    m_Tiles.resize(height, std::vector<Tile>(width));
}

WorldGenerator::~WorldGenerator() {}

void WorldGenerator::Generate() {
    m_IsGenerating = true;
    m_Progress = 0.0f;

    GenerateHeightMap();
    m_Progress = 0.5f;

    GenerateBiomes();
    m_Progress = 1.0f;

    m_IsGenerating = false;
}

void WorldGenerator::GenerateHeightMap() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int y = 0; y < m_Height; ++y) {
        for (int x = 0; x < m_Width; ++x) {
            float height = dis(gen);
            
            if (height < 0.3f) {
                m_Tiles[y][x].type = TileType::Water;
            }
            else if (height < 0.7f) {
                m_Tiles[y][x].type = TileType::Grass;
            }
            else {
                m_Tiles[y][x].type = TileType::Mountain;
            }
            
            m_Tiles[y][x].color = GetTileColor(m_Tiles[y][x].type);
        }
    }
}

void WorldGenerator::GenerateBiomes() {
    // В этой версии мы просто используем цвета для разных типов тайлов
    for (int y = 0; y < m_Height; ++y) {
        for (int x = 0; x < m_Width; ++x) {
            m_Tiles[y][x].color = GetTileColor(m_Tiles[y][x].type);
        }
    }
}

Color WorldGenerator::GetTileColor(TileType type) const {
    switch (type) {
        case TileType::Water:
            return Color(0, 0, 255);  // Синий
        case TileType::Grass:
            return Color(0, 255, 0);  // Зелёный
        case TileType::Mountain:
            return Color(128, 128, 128);  // Серый
        default:
            return Color(0, 0, 0);  // Чёрный
    }
}

void WorldGenerator::Render(IRenderer* renderer) {
    const int tileSize = 6;  // Размер каждого тайла в пикселях
    const int offsetX = 10;  // Отступ от левого края
    const int offsetY = 3;   // Отступ от верхнего края

    for (int y = 0; y < m_Height; ++y) {
        for (int x = 0; x < m_Width; ++x) {
            const Tile& tile = m_Tiles[y][x];
            renderer->FillRect(
                offsetX + x * tileSize,
                offsetY + y * tileSize,
                tileSize - 1,
                tileSize - 1,
                tile.color
            );
        }
    }
} 