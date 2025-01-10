cmake_minimum_required(VERSION 3.10)
project(DegraEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим необходимые пакеты
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLFW3 REQUIRED)

# Добавляем подпроекты
add_subdirectory(src)

# Опции сборки
option(USE_LEGACY_RENDERER "Use OpenGL legacy renderer" OFF)

# Определяем основные переменные
if(USE_LEGACY_RENDERER)
    add_definitions(-DUSE_LEGACY_RENDERER)
endif() 