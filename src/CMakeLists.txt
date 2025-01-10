# Собираем список исходных файлов
file(GLOB_RECURSE SOURCES 
    "*.cpp"
    "Audio/*.cpp"
    "Menu/*.cpp"
    "Renderer/*.cpp"
    "World/*.cpp"
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Добавляем пути для поиска заголовочных файлов
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
)

# Если FMOD найден, добавляем его пути
if(USE_FMOD AND EXISTS "${FMOD_ROOT}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${FMOD_INCLUDE_DIR})
endif()

# Подключаем библиотеки
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    SDL2::SDL2 
    SDL2::SDL2main
    ${SDL2_TTF_LIBRARY}
    ${GLEW_LIBRARY}
    glfw
    OpenGL::GL
)

# Если FMOD найден, подключаем его библиотеку
if(USE_FMOD AND EXISTS "${FMOD_ROOT}")
    target_link_libraries(${PROJECT_NAME} PRIVATE ${FMOD_LIBRARY})
endif()

# Копируем необходимые DLL файлы
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${GLEW_BINARY}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# Копируем SDL2_ttf DLL
if(EXISTS "${SDL2_TTF_BINARY}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2_TTF_BINARY}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# Если FMOD найден, копируем его DLL
if(USE_FMOD AND EXISTS "${FMOD_ROOT}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${FMOD_BINARY}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif() 