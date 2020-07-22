#include "shader.h"
#include <fstream>
#include <sstream>
#include <cstdio>

// TODO: Clean up reading code
Shader LoadFromFile(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    // Load files into memory
    std::ifstream vsRaw(vertexShaderPath);
    std::stringstream vsStream;
    vsStream << vsRaw.rdbuf();
    vsRaw.close();

    std::string vsString = vsStream.str();
    const char* vsSource = vsString.c_str();

    std::ifstream fsRaw(fragmentShaderPath);
    std::stringstream fsStream;
    fsStream << fsRaw.rdbuf();
    fsRaw.close();

    std::string fsString = fsStream.str();
    const char* fsSource = fsString.c_str();

    // Create vertex shader object
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsSource, 0);
    glCompileShader(vertex);

    // Check compilation errors
    GLint compiled;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &compiled);
    if(compiled != GL_TRUE)
    {
        GLsizei length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vertex, 1024, &length, message);
        printf("%s\n", message);
    }

    // Create fragment shader object
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fsSource, 0);
    glCompileShader(fragment);

    // Check compilation errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &compiled);
    if(compiled != GL_TRUE)
    {
        GLsizei length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fragment, 1024, &length, message);
        printf("%s\n", message);
    }

    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // Clean up
    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return { ID };
}
