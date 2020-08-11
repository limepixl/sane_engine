#include "resource_management.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

        if(channels == 1)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
        else if(channels == 2)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
        else if(channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if(channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        else
        {
            printf("Image loaded isn't in any of the supported formats!\n(Supported: RGB, RGBA)\n");
            stbi_image_free(data);

            glDeleteTextures(1, &ID);
            exit(-1);
        }

        glGenerateMipmap(ID);

        printf("Loaded texture file at: %s\n", path);
        stbi_image_free(data);

        return { width, height, channels, ID, index };
    }

    printf("Failed to load texture from image at path: %s\n", path);
    exit(-1);
}

Texture LoadCubemapFromFile(std::string* paths, int numStrings, unsigned int index)
{
    // Load image from file
    stbi_set_flip_vertically_on_load(false);

    GLuint ID;
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int width, height, channels;
    unsigned char* data;
    for(int i = 0; i < numStrings; i++)
    {
        data = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
        if(data)
        {
            if(channels == 3)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else if(channels == 4)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(ID);
            stbi_image_free(data);
        }
        else
        {
            printf("Failed to load cubemap texture at path: %s\n", paths[i].c_str());
            stbi_image_free(data);
        }
    }

    return {0, 0, 3, ID, index};
}

Shader LoadShaderFromFile(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath)
{
    bool geometryShader = geometryShaderPath != nullptr;

    // Load files into memory
    FILE* vsRaw = fopen(vertexShaderPath, "rb");
    if(!vsRaw)
    {
        printf("Failed to open file at path: %s\n", vertexShaderPath);
        exit(-1);
    }

    fseek(vsRaw, 0, SEEK_END);
    long size = ftell(vsRaw);
    rewind(vsRaw);

    char* vsBuffer = new char[size + 1];
    size_t readSize = fread(vsBuffer, 1, size, vsRaw);
    vsBuffer[size] = '\0';
    rewind(vsRaw);

    // Create vertex shader object
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vsBuffer, 0);
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

    FILE* fsRaw = fopen(fragmentShaderPath, "rb");
    if(!fsRaw)
    {
        printf("Failed to open file at path: %s\n", fragmentShaderPath);
        exit(-1);
    }

    fseek(fsRaw, 0, SEEK_END);
    size = ftell(fsRaw);
    rewind(fsRaw);

    char* fsBuffer = new char[size + 1];
    readSize = fread(fsBuffer, 1, size, fsRaw);
    fsBuffer[size] = '\0';
    rewind(fsRaw);

    // Create fragment shader object
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fsBuffer, 0);
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

    GLuint geometry = 0;
    if(geometryShader)
    {
        FILE* gsRaw = fopen(geometryShaderPath, "rb");
        if(!gsRaw)
        {
            printf("Failed to open file at path: %s\n", geometryShaderPath);
            exit(-1);
        }

        fseek(gsRaw, 0, SEEK_END);
        size = ftell(gsRaw);
        rewind(gsRaw);

        char* gsBuffer = new char[size + 1];
        readSize = fread(gsBuffer, 1, size, gsRaw);
        gsBuffer[size] = '\0';
        rewind(gsRaw);

        // Create vertex shader object
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gsBuffer, 0);
        glCompileShader(geometry);

        // Check compilation errors
        glGetShaderiv(geometry, GL_COMPILE_STATUS, &compiled);
        if(compiled != GL_TRUE)
        {
            GLsizei length = 0;
            GLchar message[1024];
            glGetShaderInfoLog(geometry, 1024, &length, message);
            printf("Geometry Shader Compilation Errors:\n%s\n", message);
        }

        glAttachShader(ID, geometry);

        fclose(gsRaw);
        delete[] gsBuffer;
    }

    glLinkProgram(ID);

    // Get uniform names and locations from program 
    std::unordered_map<std::string, uint32_t> uniforms;

    int uniformCount = 0;
    glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &uniformCount);

    if(uniformCount)
    {
        int maxNameLength = 0;
        GLsizei length = 0, count = 0;
        GLenum type = GL_NONE;
        glGetProgramiv(ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

        char* buffer = new char[maxNameLength];
        for(int i = 0; i < uniformCount; i++)
        {
            glGetActiveUniform(ID, i, maxNameLength, &length, &count, &type, buffer);

            uniforms[std::string(buffer, length)] = glGetUniformLocation(ID, buffer);
        }

        delete[] buffer;
    }

    // Clean up
    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(geometryShader)
    {
        glDetachShader(ID, geometry);
        glDeleteShader(geometry);
    }

    fclose(vsRaw);
    fclose(fsRaw);
    delete[] vsBuffer;
    delete[] fsBuffer;

    return { ID, uniforms };
}

Mesh LoadMeshFromOBJ(const char* path)
{
    FILE* objRaw = fopen(path, "r");
    if(!objRaw)
    {
        printf("Failed to open OBJ file at path: %s\n", path);
        exit(-1);
    }

    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;

    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> textureIndices;
    std::vector<unsigned int> normalIndices;

    char buffer[100];
    while(fscanf(objRaw, "%s ", buffer) != EOF)
    {
        buffer[strlen(buffer)] = '\0';
        if(!strcmp(buffer, "v"))
        {
            float x, y, z;
            if(fscanf(objRaw, "%f %f %f\n", &x, &y, &z) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        else if(!strcmp(buffer, "vt"))
        {
            float u, v;
            if(fscanf(objRaw, "%f %f\n", &u, &v) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            uvs.push_back(u);
            uvs.push_back(v);
        }
        else if(!strcmp(buffer, "vn"))
        {
            float x, y, z;
            if(fscanf(objRaw, "%f %f %f\n", &x, &y, &z) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
        else if(!strcmp(buffer, "f"))
        {
            int v1, v2, v3, t1, t2, t3, n1, n2, n3;
            if(fscanf(objRaw, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            vertexIndices.push_back(v1 - 1);
            vertexIndices.push_back(v2 - 1);
            vertexIndices.push_back(v3 - 1);

            textureIndices.push_back(t1 - 1);
            textureIndices.push_back(t2 - 1);
            textureIndices.push_back(t3 - 1);

            normalIndices.push_back(n1 - 1);
            normalIndices.push_back(n2 - 1);
            normalIndices.push_back(n3 - 1);
        }
    }

    fclose(objRaw);

    std::vector<float> finalVertices;
    std::vector<float> finalUVs;
    std::vector<float> finalNormals;
    for(size_t i = 0; i < vertexIndices.size(); i++)
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

    printf("Loaded mesh from .obj file at: %s\n", path);

    return GenerateMesh(finalVertices.data(), (int)finalVertices.size(), finalUVs.data(), (int)finalUVs.size(), finalNormals.data(), (int)finalNormals.size());
}

MeshIndexed LoadMeshIndexedFromOBJ(const char* path)
{
    FILE* objRaw = fopen(path, "r");
    if(!objRaw)
    {
        printf("Failed to open OBJ file at path: %s\n", path);
        exit(-1);
    }

    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;

    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> textureIndices;
    std::vector<unsigned int> normalIndices;

    std::map<std::vector<int>, int> indexMap;

    char buffer[100];
    while(fscanf(objRaw, "%s ", buffer) != EOF)
    {
        buffer[strlen(buffer)] = '\0';
        if(!strcmp(buffer, "v"))
        {
            float x, y, z;
            if(fscanf(objRaw, "%f %f %f\n", &x, &y, &z) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        else if(!strcmp(buffer, "vt"))
        {
            float u, v;
            if(fscanf(objRaw, "%f %f\n", &u, &v) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            uvs.push_back(u);
            uvs.push_back(v);
        }
        else if(!strcmp(buffer, "vn"))
        {
            float x, y, z;
            if(fscanf(objRaw, "%f %f %f\n", &x, &y, &z) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
        else if(!strcmp(buffer, "f"))
        {
            int v1, v2, v3, t1, t2, t3, n1, n2, n3;
            if(fscanf(objRaw, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3) == EOF)
                printf("Invalid format detected in OBJ file!\n");

            vertexIndices.push_back(v1 - 1);
            vertexIndices.push_back(v2 - 1);
            vertexIndices.push_back(v3 - 1);

            textureIndices.push_back(t1 - 1);
            textureIndices.push_back(t2 - 1);
            textureIndices.push_back(t3 - 1);

            normalIndices.push_back(n1 - 1);
            normalIndices.push_back(n2 - 1);
            normalIndices.push_back(n3 - 1);
        }
    }

    fclose(objRaw);

    // A big thank you to this answer for the algorithm.
    // https://stackoverflow.com/a/23356738
    std::vector<float> finalVertices;
    std::vector<float> finalUVs;
    std::vector<float> finalNormals;
    std::vector<unsigned int> finalIndices;

    int nextCombinedIndex = 0;
    for(size_t i = 0; i < vertexIndices.size(); i++)
    {
        int combinedIndex;
        int vindex = vertexIndices[i];
        int uvindex = textureIndices[i];
        int nindex = normalIndices[i];

        std::vector<int> indexData{ vindex, uvindex, nindex };

        // If map contains this index triplet
        if(indexMap.count(indexData) != 0)
            combinedIndex = indexMap[indexData];
        else
        {
            combinedIndex = nextCombinedIndex;
            indexMap[indexData] = combinedIndex;
            nextCombinedIndex++;

            finalVertices.push_back(vertices[3 * vindex]);
            finalVertices.push_back(vertices[3 * vindex + 1]);
            finalVertices.push_back(vertices[3 * vindex + 2]);

            finalUVs.push_back(uvs[2 * uvindex]);
            finalUVs.push_back(uvs[2 * uvindex + 1]);

            finalNormals.push_back(normals[3 * nindex]);
            finalNormals.push_back(normals[3 * nindex + 1]);
            finalNormals.push_back(normals[3 * nindex + 2]);
        }

        finalIndices.push_back(combinedIndex);
    }

    printf("Loaded indexed mesh from .obj file at: %s\n", path);

    return GenerateMeshIndexed(finalVertices.data(), (int)finalVertices.size(), finalIndices.data(), (int)finalIndices.size(), finalUVs.data(), (int)finalUVs.size(), finalNormals.data(), (int)finalNormals.size());
}

// Simple bubble sort based on the mesh index
void SortEntitiesByMesh(std::vector<Entity>& entities)
{
    for(size_t i = 0; i < entities.size(); i++)
        for(size_t j = 0; j < entities.size() - 1 - i; i++)
        {
            if(entities[j].meshIndex > entities[j + 1].meshIndex)
            {
                Entity tmp = entities[j];
                entities[j] = entities[j + 1];
                entities[j + 1] = tmp;
            }
        }
}

Scene LoadSceneFromFile(const char* path)
{
    std::vector<MeshIndexed> meshes;
    std::vector<Texture> textures;
    std::vector<Entity> entities;
    std::vector<glm::vec3> lights;
    
    FILE* rawScene = fopen(path, "r");
    if(!rawScene)
    {
        printf("Failed to open scene file at path: %s\n", path);
        exit(-1);
    }

    char token[100];;
    while(true)
    {
        if(fscanf(rawScene, "%s", token) == EOF)
            break;

        token[strlen(token)] = '\0';
        if(!strcmp(token, "m"))
        {
            if(fscanf(rawScene, "%s\n", token) == EOF)
                printf("Invalid format detected in scene file!\n");
            char prepend[100] = "res/models/";
            MeshIndexed tmp = LoadMeshIndexedFromOBJ(strcat(prepend, token));
            meshes.push_back(tmp);
            continue;
        } 

        if(!strcmp(token, "t"))
        {
            if(fscanf(rawScene, "%s\n", token) == EOF)
                printf("Invalid format detected in scene file!\n");
            char prepend[100] = "res/image/";
            textures.push_back(LoadTextureFromFile(strcat(prepend, token), Texture::GlobalTextureCount++));
            continue;
        }

        if(!strcmp(token, "e"))
        {
            unsigned int modelIndex, diffuseIndex, specularIndex;

            glm::vec3 pos; glm::vec3 rot; glm::vec3 scale;
            if(fscanf(rawScene, "%d %d %d %f %f %f %f %f %f %f %f %f ;\n", &modelIndex, &diffuseIndex, &specularIndex, &pos.x, &pos.y, &pos.z, &rot.x, &rot.y, &rot.z, &scale.x, &scale.y, &scale.z) == EOF)
                printf("Invalid format detected in scene file!\n");

            entities.push_back({ modelIndex, diffuseIndex, specularIndex, pos, rot, scale });
            continue;
        }

        if(!strcmp(token, "l"))
        {
            glm::vec3 lightPos;
            if(fscanf(rawScene, "%f %f %f\n", &lightPos.x, &lightPos.y, &lightPos.z) == EOF)
                printf("Invalid format detected in scene file!\n");
            lights.push_back(lightPos);
            continue;
        }
    }

    fclose(rawScene);

    SortEntitiesByMesh(entities);

    return { meshes, textures, entities, lights };
}

