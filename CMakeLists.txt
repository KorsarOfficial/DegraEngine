cmake_minimum_required(VERSION 3.10)
project(DegraEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к библиотекам
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/external/SDL2")
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/external/glfw")
set(GLEW_DIR "${CMAKE_SOURCE_DIR}/external/glew")
set(FMOD_DIR "${CMAKE_SOURCE_DIR}/external/fmod")
set(SDL2_IMAGE_DIR "${CMAKE_SOURCE_DIR}/external/SDL2_image")

# Находим OpenGL
find_package(OpenGL REQUIRED)

# Настройка SDL2
set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include")
set(SDL2_LIBRARY "${SDL2_DIR}/lib/x64/SDL2.lib")
set(SDL2_MAIN_LIBRARY "${SDL2_DIR}/lib/x64/SDL2main.lib")

# Настройка SDL2_image
set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_IMAGE_DIR}/include")
set(SDL2_IMAGE_LIBRARY "${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.lib")

# Настройка GLFW
set(GLFW_INCLUDE_DIR "${GLFW_DIR}/include")
set(GLFW_LIBRARY "${GLFW_DIR}/lib-vc2022/glfw3.lib")

# Настройка GLEW
set(GLEW_INCLUDE_DIR "${GLEW_DIR}/include")
set(GLEW_LIBRARY "${GLEW_DIR}/lib/Release/x64/glew32s.lib")

# Настройка FMOD
set(FMOD_INCLUDE_DIR "${FMOD_DIR}/include")
set(FMOD_LIBRARY_DIR "${FMOD_DIR}/lib")
if(WIN32)
    set(FMOD_LIBRARIES 
        "${FMOD_LIBRARY_DIR}/fmod_vc.lib"
        "${FMOD_LIBRARY_DIR}/fmodL_vc.lib"
    )
endif()

# Опции сборки
option(USE_LEGACY_RENDERER "Use OpenGL legacy renderer" OFF)

# Определяем основные переменные
if(USE_LEGACY_RENDERER)
    add_definitions(-DUSE_LEGACY_RENDERER)
endif()

# Добавляем подпроекты
add_subdirectory(src) 