//
// Created by S.Morgenthaler on 03/02/2023.
//

#include "TextureSampler.h"

TextureSampler::TextureSampler(): _textureId(
    []() {
        GLuint texture;
        glGenTextures(1, &texture);
        return texture;
    }()
) {
}

void TextureSampler::bind() const {
    glBindTexture(GL_TEXTURE_2D, _textureId);
}
void TextureSampler::bindNoTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureSampler::setActiveTexture(GLint index) {
    glActiveTexture(GL_TEXTURE0 + index);
}

GLuint TextureSampler::getId() const {
    return _textureId;
}

TextureSampler::~TextureSampler() {
    glDeleteTextures(1, &_textureId);
}
