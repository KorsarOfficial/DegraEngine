cmake_minimum_required(VERSION 3.10)
project(DegraEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим необходимые пакеты
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)

# Добавляем FMOD
set(FMOD_ROOT "${CMAKE_SOURCE_DIR}/external/fmod")
set(FMOD_INCLUDE_DIR "${FMOD_ROOT}/include")
set(FMOD_LIBRARY_DIR "${FMOD_ROOT}/lib")

if(WIN32)
    set(FMOD_LIBRARIES 
        "${FMOD_LIBRARY_DIR}/fmod_vc.lib"
        "${FMOD_LIBRARY_DIR}/fmodL_vc.lib"
    )
else()
    set(FMOD_LIBRARIES 
        "${FMOD_LIBRARY_DIR}/libfmod.so"
        "${FMOD_LIBRARY_DIR}/libfmodL.so"
    )
endif()

# Добавляем подпроекты
add_subdirectory(src)

# Опции сборки
option(USE_LEGACY_RENDERER "Use OpenGL legacy renderer" OFF)

# Определяем основные переменные
if(USE_LEGACY_RENDERER)
    add_definitions(-DUSE_LEGACY_RENDERER)
endif()

# Копируем динамические библиотеки в директорию сборки
if(WIN32)
    add_custom_command(TARGET DegraEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${FMOD_LIBRARY_DIR}/fmod.dll"
            "${FMOD_LIBRARY_DIR}/fmodL.dll"
            $<TARGET_FILE_DIR:DegraEngine>
    )
endif() 