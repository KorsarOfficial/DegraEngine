#pragma once

#include "../Renderer/IRenderer.hpp"
#include "../World/WorldGenerator.hpp"
#include <SDL2/SDL_keycode.h>
#include <memory>

class Menu {
public:
    enum class State {
        MainMenu,
        Settings,
        WorldGeneration,
        ViewWorld
    };

    Menu(IRenderer* renderer);
    ~Menu();

    void Update();
    void Render();
    void HandleInput(SDL_Keycode key);

private:
    void RenderMainMenu();
    void RenderSettings();
    void RenderWorldGeneration();
    void RenderWorld();

    void StartWorldGeneration();
    void ReturnToMainMenu();

    IRenderer* m_Renderer;
    State m_CurrentState;
    std::unique_ptr<WorldGenerator> m_WorldGenerator;
    float m_GenerationProgress;
}; 