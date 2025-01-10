cmake_minimum_required(VERSION 3.15)
project(DegraEngine)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка vcpkg
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Находим необходимые пакеты
find_package(OpenGL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

# Настройка SDL2_ttf
set(SDL2_TTF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_ttf" CACHE PATH "Path to SDL2_ttf")
if(EXISTS "${SDL2_TTF_PATH}")
    set(SDL2_TTF_INCLUDE_DIR "${SDL2_TTF_PATH}/include")
    set(SDL2_TTF_LIBRARY "${SDL2_TTF_PATH}/lib/x64/SDL2_ttf.lib")
    set(SDL2_TTF_BINARY "${SDL2_TTF_PATH}/bin/x64/SDL2_ttf.dll")
endif()

# Настройка GLEW
set(GLEW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glew/include")
set(GLEW_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glew/lib/Release/x64")
set(GLEW_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glew/bin/Release/x64")
set(GLEW_LIBRARY "${GLEW_LIBRARY_DIR}/glew32.lib")
set(GLEW_BINARY "${GLEW_BINARY_DIR}/glew32.dll")

# Опционально ищем FMOD
option(USE_FMOD "Use FMOD for audio" OFF)
if(USE_FMOD)
    set(FMOD_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/fmod" CACHE PATH "Path to FMOD SDK")
    if(EXISTS "${FMOD_ROOT}")
        set(FMOD_INCLUDE_DIR "${FMOD_ROOT}/inc")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(FMOD_LIBRARY_DIR "${FMOD_ROOT}/lib/x64")
        else()
            set(FMOD_LIBRARY_DIR "${FMOD_ROOT}/lib/x86")
        endif()
        set(FMOD_LIBRARY "${FMOD_LIBRARY_DIR}/fmod_vc.lib")
        set(FMOD_BINARY "${FMOD_LIBRARY_DIR}/fmod.dll")
        add_definitions(-DUSE_FMOD)
    endif()
endif()

find_package(glfw3 CONFIG REQUIRED)

# Опция для использования legacy рендерера
option(USE_LEGACY_RENDERER "Use OpenGL legacy renderer" OFF)

# Добавляем подкаталог с исходными файлами
add_subdirectory(src) 