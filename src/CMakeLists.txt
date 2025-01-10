add_executable(DegraEngine
    main.cpp
    Engine.cpp
    Window.cpp
    Renderer/Renderer.cpp
    Renderer/OpenGLRenderer.cpp
    Renderer/SDLRenderer.cpp
    Audio/AudioSystem.cpp
)

target_include_directories(DegraEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENGL_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${FMOD_INCLUDE_DIR}
)

target_link_libraries(DegraEngine
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${FMOD_LIBRARIES}
) 