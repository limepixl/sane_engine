#pragma once
#include <Texture/texture.h>
#include <Shader/shader.h>
#include <Mesh/mesh.h>
#include <Scene/scene.h>

Texture LoadTextureFromFile(const char* path, unsigned int index);
Shader LoadShaderFromFile(const char* vertexShaderPath, const char* fragmentShaderPath);
Mesh LoadMeshFromOBJ(const char* path);
Scene LoadSceneFromFile(const char* path);