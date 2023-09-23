#ifdef __linux__
#include <GL/glew.h>
#elif __MACH__
#include <OpenGL/gl.h>
#endif


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include "Mesh.hpp"
#include "ObjParser.hpp"
#include "Renderer.hpp"
#include "Renderable.hpp"

static std::string load_shader(const char* filePath)
{
    std::ifstream stream(filePath);
    std::string line;
    std::stringstream ss;
    while (getline(stream,line))
    {
        ss << line << '\n';
    }
    return ss.str();
}

static unsigned int compile_shader(unsigned int type, const char* src)
{
    unsigned int id = glCreateShader(type);

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length *sizeof(char));
        glGetShaderInfoLog(id, GL_INFO_LOG_LENGTH, &length,message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment ") << "shader" << "\n";
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int create_shader(const char* vertexShader, const char* fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vShader = compile_shader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fShader = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program,vShader);
    glAttachShader(program,fShader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}

int main()
{
    slib::texture texture = slib::DecodePng("resources/texture3.png");
    Mesh* cubeMesh = ObjParser::ParseObj("resources/cube.obj", texture);
    Renderable cubeInstance(*cubeMesh, {0, 0, -5}, { 0, 0, 0 }, { 1, 1 ,1 }, { 100, 100, 255 }, 
                            cubeMesh->vertices, cubeMesh->normals);
    slib::Camera camera = { {0,0,0}, {0,0,0}, {0,0,-1}, {0,1,0}, zFar, zNear}; 
    Renderer renderer(&camera);
    renderer.AddRenderable(cubeInstance);
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }
    
    glm::vec3 test;
    test.x = 1;
    
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Fun", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

#ifdef __linux__
    if (glewInit() != GLEW_OK) {
        std::cout << "Error " << std::endl;
    }
#endif

    std::array<float, 6> position = {
        0,    0.5f,
        0.5f ,-0.5f,
        -0.5f, -0.5f
    };
    
    unsigned int buffer;
    glGenBuffers(1, &buffer); // Generate
    glBindBuffer(GL_ARRAY_BUFFER, buffer ); // Select buffer
    glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(position[0]), position.data(), GL_STATIC_DRAW); // Set the buffer data into the buffer
    glEnableVertexAttribArray(0); // Enable the attribute of the array from the selected buffer's 0 index
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, nullptr);
    
    auto vertexShader = load_shader("resources/vertex.shader");
    auto fragShader = load_shader("resources/fragment.shader");
    
    unsigned int shader = create_shader(vertexShader.c_str(), fragShader.c_str());
    glUseProgram(shader);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0 , 3);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}