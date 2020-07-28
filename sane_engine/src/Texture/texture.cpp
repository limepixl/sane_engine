#include "texture.h"

void BindTexture(Texture& texture)
{
    glActiveTexture(GL_TEXTURE0 + texture.index);
    glBindTexture(GL_TEXTURE_2D, texture.ID);
}

void UnbindTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Clean(Texture& t)
{
    glDeleteTextures(1, &t.ID);
}
