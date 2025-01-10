#include "Menu.hpp"
#include <stdexcept>
#include <iostream>

Menu::Menu(IRenderer* renderer) : m_Renderer(nullptr) {
    std::cout << "Menu constructor called" << std::endl;
    
    if (!renderer) {
        throw std::runtime_error("Invalid renderer pointer");
    }
    
    m_Renderer = renderer;
    std::cout << "Menu initialized with renderer" << std::endl;
}

Menu::~Menu() {
    std::cout << "Menu destructor called" << std::endl;
    
    // Не удаляем m_Renderer, так как он нам не принадлежит
    m_Renderer = nullptr;
    std::cout << "Menu destroyed" << std::endl;
}

void Menu::Update() {
    if (!m_Renderer) {
        std::cerr << "Cannot update menu: renderer is null" << std::endl;
        return;
    }
    // TODO: Реализовать обновление меню
}

void Menu::Render() {
    if (!m_Renderer) {
        std::cerr << "Cannot render menu: renderer is null" << std::endl;
        return;
    }

    try {
        // Рендерим заголовок
        m_Renderer->RenderUTF8Text("Degra Engine", 10, 10, Color(255, 255, 255));
        
        // Рендерим пункты меню
        m_Renderer->RenderUTF8Text("1. Начать игру", 10, 40, Color(200, 200, 200));
        m_Renderer->RenderUTF8Text("2. Настройки", 10, 70, Color(200, 200, 200));
        m_Renderer->RenderUTF8Text("3. Выход", 10, 100, Color(200, 200, 200));
    }
    catch (const std::exception& e) {
        std::cerr << "Error rendering menu: " << e.what() << std::endl;
    }
} 