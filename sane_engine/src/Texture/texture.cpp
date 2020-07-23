#include "texture.h"

void BindTexture(Texture* texture)
{
    glActiveTexture(GL_TEXTURE0 + texture->index);
    glBindTexture(GL_TEXTURE_2D, texture->ID);
}
