//
// Created by S.Morgenthaler on 03/02/2023.
//

#include "TextureSampler.h"

void TextureSampler::bind(GLuint textureId) {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void TextureSampler::setActiveTexture(GLint index) {
    glActiveTexture(GL_TEXTURE0 + index);
}
