# Finding opengl, glfw, glew, glut, and possibly freetype and even freeglut
find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm REQUIRED)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)

find_package(ImGuizmo REQUIRED)
# include_directories(/usr/local/include/glad/include/glad)
