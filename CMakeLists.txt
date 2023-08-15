cmake_minimum_required(VERSION 3.26)
project(OpenGLTomfoolery)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
find_package(glfw3 3.3.8 REQUIRED)

add_executable(OpenGLTomfoolery main.cpp)
target_link_libraries(OpenGLTomfoolery glfw)