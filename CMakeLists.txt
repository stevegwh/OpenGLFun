cmake_minimum_required(VERSION 3.26)
project(OpenGLTomfoolery)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3.8 REQUIRED)
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
    add_executable(OpenGLTomfoolery main.cpp)
    target_link_libraries(OpenGLTomfoolery glfw)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    find_package(glfw3 3.3.8 REQUIRED)
    find_package(OpenGL REQUIRED)

    add_executable(OpenGLTomfoolery main.cpp)
    target_link_libraries(OpenGLTomfoolery glfw OpenGL)

endif()