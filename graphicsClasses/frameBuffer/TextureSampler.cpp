//
// Created by S.Morgenthaler on 03/02/2023.
//

#include "TextureSampler.h"

TextureSampler::TextureSampler(GLuint textureId):
    _textureId(textureId)
{
}

TextureSampler TextureSampler::createInstance(
    GLuint textureId,
    GLint index,
    const CstShaderProgram_sptr &shaderProgram,
    const std::string &name
) {
    const auto location = glGetUniformLocation(
        shaderProgram->getHandle(),
        name.c_str()
    );
    glUniform1i(location, index);
    return TextureSampler(textureId);
}

void TextureSampler::bind() const {
    glBindTexture(GL_TEXTURE_2D, _textureId);
}

void TextureSampler::setActiveTexture(GLint index) {
    glActiveTexture(GL_TEXTURE0 + index);
}
