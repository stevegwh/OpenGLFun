#ifdef __linux__
#include <GL/glew.h>
#elif __MACH__
#include <OpenGL/gl.h>
#endif


#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <array>

struct ShaderProgramSource
{
    std::string VertexShader;
    std::string FragmentShader;
};

static ShaderProgramSource ParseShader(const char* filePath)
{

    enum ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream stream(filePath);
    ShaderType type = ShaderType :: NONE;
    std::string line;
    std::stringstream ss[2];
    while (getline(stream,line))
    {
        if (line.find("#shader") != std::string :: npos)
        {
            if (line.find("vertex") != std::string ::npos)
            {
                // vertex mode
                type = ShaderType::VERTEX;

            }
            else
            {
                // fragment mode
                type = ShaderType::FRAGMENT;

            }
        }
        else
        {
            ss[(int)type] << line << '\n'; // Add line to the stream string
        }
    }
    return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const char* src)
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
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment") << "shader" << "\n";
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const char* vertexShader, const char* fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

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
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }
    
    glm::vec3 test;
    test.x = 1;
    
    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

//    if (glewInit() != GLEW_OK) {
//        std::cout << "Error " << std::endl;
//    }


    std::array<float, 6> position = {
        0,    0.5f,
        0.5f ,-0.5f,
        -0.5f, -0.5f
    };
    
    unsigned int buffer;
    glGenBuffers(1, &buffer);// Generate
    glBindBuffer(GL_ARRAY_BUFFER, buffer ); // Select buffer
    glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(position[0]), position.data(), GL_STATIC_DRAW); // Set the buffer data into the buffer
    glEnableVertexAttribArray(0); // Enable the attribute of the array from the selected buffer's 0 index
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, nullptr);

    ShaderProgramSource source = ParseShader("myFirstShader.shader");
    unsigned int shader = CreateShader(source.VertexShader.c_str(), source.FragmentShader.c_str());
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