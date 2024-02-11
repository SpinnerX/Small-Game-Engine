cmake_minimum_required (VERSION 3.15)

set(PROJECT_NAME Rocket-Game)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works (in creating the compile_commands.json file)
project(${PROJECT_NAME})

include(cmake/modules.cmake)
find_package(GameEngine REQUIRED)

add_executable(
	${PROJECT_NAME}
	main.cpp
	GameLayer.cpp
	Level.cpp
	ParticleSystem.cpp
	Player.cpp
	Random.cpp
)


target_link_libraries(
    ${PROJECT_NAME}
    glm::glm
    glad::glad
    fmt::fmt
    spdlog::spdlog

    # Linking the libraries for Cocoa, OpenGL, IOKit, and glfw
    "-framework Cocoa"
    "-framework OpenGL"
    "-framework IOKit"
    glfw
    ${OPENGL_gl_LIBRARY}
    GLEW::GLEW
    imgui::imgui
    ImGuizmo::ImGuizmo
	GameEngine::GameEngine
)
