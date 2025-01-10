# Список исходных файлов
set(SOURCE_FILES
    main.cpp
    Engine.cpp
    Window.cpp
    Renderer/OpenGLRenderer.cpp
    Renderer/SDLRenderer.cpp
    Renderer/TextRenderer.cpp
)

add_executable(DegraEngine ${SOURCE_FILES})

target_include_directories(DegraEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
)

# Добавляем определения для SDL2 и GLFW
target_compile_definitions(DegraEngine PRIVATE
    SDL_MAIN_HANDLED
    GLFW_EXPOSE_NATIVE_WIN32
    GLEW_STATIC
)

target_link_libraries(DegraEngine PRIVATE
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${SDL2_LIBRARY}
    ${SDL2_MAIN_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${GLFW_LIBRARY}
)

# Копируем DLL файлы в директорию сборки
if(WIN32)
    add_custom_command(TARGET DegraEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DIR}/lib/x64/SDL2.dll"
            "${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.dll"
            $<TARGET_FILE_DIR:DegraEngine>
    )
endif() 