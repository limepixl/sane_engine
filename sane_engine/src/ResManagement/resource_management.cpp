#include "resource_management.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// GLOBAL TODO: Clean up file reading code

Texture LoadTextureFromFile(const char* path, unsigned int index)
{
    // Load image from file
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if(data)
    {
        GLuint ID;
        glGenTextures(1, &ID);

        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if(channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if(channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
            printf("Image loaded isn't in any of the supported formats!\n(Supported: RGB, RGBA)\n");

        glGenerateMipmap(ID);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);

        return { width, height, channels, ID, index };
    }

    printf("Failed to load texture from image at path: %s\n", path);
    return {};
}

Shader LoadShaderFromFile(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    // Load files into memory
    std::ifstream vsRaw(vertexShaderPath);
    if(!vsRaw.is_open())
        printf("Failed to open file at path: %s\n", vertexShaderPath);

    std::stringstream vsStream;
    vsStream << vsRaw.rdbuf();
    vsRaw.close();

    std::string vsString = vsStream.str();
    const char* vsSource = vsString.c_str();

    std::ifstream fsRaw(fragmentShaderPath);
    if(!fsRaw.is_open())
        printf("Failed to open file at path: %s\n", fragmentShaderPath);

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
        printf("Vertex Shader Compilation Errors:\n%s\n", message);
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
        printf("Fragment Shader Compilation Errors:\n%s\n", message);
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

std::vector<unsigned int> stripFaceDataFromToken(std::string& token)
{
    std::vector<unsigned int> indices;

    int lastChar = -1;
    for(int i = 0; i < token.size(); i++)
    {
        if(token[i] == '/')
        {
            indices.push_back((unsigned int)std::atoi(std::string(token.begin() + size_t(lastChar + 1), token.begin() + i).c_str()));
            lastChar = i;
        }

        if(i == token.size() - 1)
        {
            indices.push_back((unsigned int)std::atoi(std::string(token.begin() + size_t(lastChar + 1), token.end()).c_str()));
        }
    }

    return indices;
}

// TODO: Create an indexed version of this
Mesh LoadMeshFromOBJ(const char* path)
{
    std::ifstream objRaw(path);
    if(!objRaw.is_open())
    {
        printf("Failed to open OBJ file at path: %s\n", path);
        return {};
    }

    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;

    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> textureIndices;
    std::vector<unsigned int> normalIndices;

    std::string token;
    while(objRaw >> token)
    {
        if(token == "v")
        {
            std::string x, y, z;
            objRaw >> x >> y >> z;

            vertices.push_back((float)std::stof(x));
            vertices.push_back((float)std::stof(y));
            vertices.push_back((float)std::stof(z));
        }
        else if(token == "vt")
        {
            std::string u, v;
            objRaw >> u >> v;

            uvs.push_back((float)std::stof(u));
            uvs.push_back((float)std::stof(v));
        }
        else if(token == "vn")
        {
            std::string x, y, z;
            objRaw >> x >> y >> z;

            normals.push_back((float)std::stof(x));
            normals.push_back((float)std::stof(y));
            normals.push_back((float)std::stof(z));
        }
        else if(token == "f")
        {
            std::string first, second, third;
            objRaw >> first >> second >> third;

            std::vector<unsigned int> firstVertexData = stripFaceDataFromToken(first);
            std::vector<unsigned int> secondVertexData = stripFaceDataFromToken(second);
            std::vector<unsigned int> thirdVertexData = stripFaceDataFromToken(third);

            vertexIndices.push_back(firstVertexData[0] - 1);
            vertexIndices.push_back(secondVertexData[0] - 1);
            vertexIndices.push_back(thirdVertexData[0] - 1);

            textureIndices.push_back(firstVertexData[1] - 1);
            textureIndices.push_back(secondVertexData[1] - 1);
            textureIndices.push_back(thirdVertexData[1] - 1);

            normalIndices.push_back(firstVertexData[2] - 1);
            normalIndices.push_back(secondVertexData[2] - 1);
            normalIndices.push_back(thirdVertexData[2] - 1);
        }
    }

    std::vector<float> finalVertices;
    std::vector<float> finalUVs;
    std::vector<float> finalNormals;
    for(int i = 0; i < vertexIndices.size(); i++)
    {
        finalVertices.push_back(vertices[3 * vertexIndices[i]]);
        finalVertices.push_back(vertices[3 * vertexIndices[i] + 1]);
        finalVertices.push_back(vertices[3 * vertexIndices[i] + 2]);

        finalUVs.push_back(uvs[2 * textureIndices[i]]);
        finalUVs.push_back(uvs[2 * textureIndices[i] + 1]);

        finalNormals.push_back(normals[3 * normalIndices[i]]);
        finalNormals.push_back(normals[3 * normalIndices[i] + 1]);
        finalNormals.push_back(normals[3 * normalIndices[i] + 2]);
    }

    return GenerateMesh(finalVertices.data(), (int)finalVertices.size(), finalUVs.data(), (int)finalUVs.size());
}

