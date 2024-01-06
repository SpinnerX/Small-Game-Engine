cmake_minimum_required (VERSION 3.15)

set(PROJECT_NAME OneHourGame)

project(${PROJECT_NAME})

# Create the executable
add_executable(${PROJECT_NAME} Game.cpp GameLayer.cpp Player.cpp Level.cpp Random.cpp ParticleSystem.cpp)

# Log::Log1 is the alias we made when we created an alias in the Logger CMakeLists.txt file
target_link_libraries(
    ${PROJECT_NAME}
    Engine::Engine
)

