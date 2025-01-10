#include "Engine.hpp"
#include <iostream>
#include <locale>
#include <codecvt>
#include <cstdlib>
#include <SDL.h>
#include <SDL_ttf.h>

// Включаем отладочную информацию для утечек памяти
#ifdef _DEBUG
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
    #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #define new DEBUG_NEW
#endif

class SDLInitializer {
public:
    SDLInitializer() {
        std::cout << "Initializing SDL..." << std::endl;
        
        // Проверяем, не был ли SDL уже инициализирован
        Uint32 subsystems = SDL_WasInit(SDL_INIT_EVERYTHING);
        std::cout << "Current SDL subsystems: " << subsystems << std::endl;
        
        // Инициализируем SDL
        if (!(subsystems & SDL_INIT_VIDEO)) {
            std::cout << "Initializing SDL Video..." << std::endl;
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
                throw std::runtime_error(std::string("SDL initialization failed: ") + SDL_GetError());
            }
        }

        // Инициализируем TTF
        std::cout << "Initializing SDL_ttf..." << std::endl;
        if (!TTF_WasInit()) {
            if (TTF_Init() == -1) {
                std::cout << "TTF_Init failed: " << TTF_GetError() << std::endl;
                SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
                SDL_Quit();
                throw std::runtime_error(std::string("TTF initialization failed: ") + TTF_GetError());
            }
        }
        
        std::cout << "SDL and TTF initialized successfully" << std::endl;
    }

    ~SDLInitializer() {
        std::cout << "Shutting down SDL subsystems..." << std::endl;
        
        // Сначала закрываем TTF
        if (TTF_WasInit()) {
            std::cout << "Quitting TTF..." << std::endl;
            TTF_Quit();
        }
        
        // Затем проверяем оставшиеся подсистемы SDL
        Uint32 subsystems = SDL_WasInit(SDL_INIT_EVERYTHING);
        std::cout << "Remaining SDL subsystems before quit: " << subsystems << std::endl;
        
        // И закрываем SDL
        if (subsystems) {
            std::cout << "Quitting SDL..." << std::endl;
            SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
            SDL_Quit();
        }
        
        std::cout << "SDL shutdown complete" << std::endl;
    }

    // Запрещаем копирование и присваивание
    SDLInitializer(const SDLInitializer&) = delete;
    SDLInitializer& operator=(const SDLInitializer&) = delete;
    SDLInitializer(SDLInitializer&&) = delete;
    SDLInitializer& operator=(SDLInitializer&&) = delete;
};

int main(int argc, char* argv[]) {
    // Включаем обнаружение утечек памяти
    #ifdef _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    #endif

    try {
        std::cout << "Запуск Degra Engine..." << std::endl;
        
        // Устанавливаем локаль для поддержки Unicode
        std::locale::global(std::locale(""));
        setlocale(LC_ALL, "");

        // Создаем все объекты в одном блоке, чтобы гарантировать правильный порядок уничтожения
        {
            std::cout << "Creating SDLInitializer..." << std::endl;
            SDLInitializer sdl;

            std::cout << "Creating Engine instance..." << std::endl;
            Engine engine;
            
            std::cout << "Initializing Engine..." << std::endl;
            if (!engine.Initialize(800, 600)) {
                std::cerr << "Ошибка инициализации движка" << std::endl;
                system("pause");
                return 1;
            }
            
            std::cout << "Starting Engine main loop..." << std::endl;
            engine.Run();
            
            std::cout << "Engine main loop finished, cleaning up..." << std::endl;
        } // Все объекты будут уничтожены здесь в правильном порядке

        std::cout << "Engine destroyed successfully" << std::endl;
        std::cout << "Движок успешно завершил работу" << std::endl;
        
        // Проверяем на утечки памяти перед выходом
        #ifdef _DEBUG
            std::cout << "Checking for memory leaks..." << std::endl;
            if (_CrtDumpMemoryLeaks()) {
                std::cout << "Memory leaks detected!" << std::endl;
            } else {
                std::cout << "No memory leaks detected" << std::endl;
            }
        #endif

        system("pause");
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        system("pause");
        return 1;
    }
    catch (...) {
        std::cerr << "Неизвестная критическая ошибка" << std::endl;
        system("pause");
        return 1;
    }
} 