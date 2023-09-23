cmake_minimum_required(VERSION 3.26)
project(OpenGLTomfoolery)

set(CMAKE_CXX_STANDARD 17)


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
    find_package(glfw3 3.3.8 REQUIRED)
    find_package(glm REQUIRED)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "-O0")
    find_package(glfw3 3.3.8 REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
endif ()

add_executable(OpenGLTomfoolery 
        main.cpp
        constants.h
        lodepng.cpp
        lodepng.h
        Mesh.hpp
        ObjParser.cpp
        ObjParser.hpp
        Renderable.hpp
        Renderer.cpp
        Renderer.hpp
        slib.cpp
        slib.h
        smath.cpp
        smath.hpp
        App.cpp
        App.h
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(OpenGLTomfoolery glfw glm::glm)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(OpenGLTomfoolery GLEW glfw OpenGL)
endif()


set (source "${CMAKE_SOURCE_DIR}/resources")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/resources")
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "symbolic link resources folder from ${source} => ${destination}"
)