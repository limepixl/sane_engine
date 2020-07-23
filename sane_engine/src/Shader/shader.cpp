#include "shader.h"
#include <glm/gtc/type_ptr.hpp>

void SetFloat(float value, const char* location, Shader& shader)
{
    int loc = glGetUniformLocation(shader.ID, location);
    glUniform1f(loc, value);
}

void SetInt(int value, const char* location, Shader& shader)
{
    int loc = glGetUniformLocation(shader.ID, location);
    glUniform1i(loc, value);
}

void SetMat4(glm::mat4& value, const char* location, Shader& shader)
{
    int loc = glGetUniformLocation(shader.ID, location);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}
