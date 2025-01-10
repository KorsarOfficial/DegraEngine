#pragma once

#include "../Renderer/IRenderer.hpp"
#include <vector>

class WorldGenerator {
public:
    enum class TileType {
        Empty,
        Grass,
        Water,
        Mountain
    };

    struct Tile {
        TileType type;
        Color color;

        Tile() : type(TileType::Empty), color(0, 0, 0) {}
    };

    WorldGenerator(int width = 100, int height = 100);
    ~WorldGenerator();

    void Generate();
    void Render(IRenderer* renderer);

    float GetProgress() const { return m_Progress; }
    bool IsGenerating() const { return m_IsGenerating; }

private:
    std::vector<std::vector<Tile>> m_Tiles;
    int m_Width;
    int m_Height;
    float m_Progress;
    bool m_IsGenerating;

    void GenerateHeightMap();
    void GenerateBiomes();
    Color GetTileColor(TileType type) const;
}; 