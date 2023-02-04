//
// Created by S.Morgenthaler on 03/02/2023.
//

#ifndef JUMPERBALLAPPLICATION_TEXTURESAMPLER_H
#define JUMPERBALLAPPLICATION_TEXTURESAMPLER_H
#include "ShaderProgram.h"


class TextureSampler {
public:

    static TextureSampler createInstance(
        GLuint textureId,
        GLint index,
        const CstShaderProgram_sptr &shaderProgram,
        const std::string &name
    );

private:
    explicit TextureSampler(
        GLuint textureId
    );
public:

    TextureSampler(const TextureSampler& textureSampler) = delete;
    TextureSampler& operator=(const TextureSampler& textureSampler) = delete;

    TextureSampler(TextureSampler&& textureSampler) = default;

    /**
     * Bind the texture.
     * Warning: Ensure that TextureSampler::setActiveTexture was previously called with
     * the index corresponding to one was used during the texture creation.
     * (index parameter passed to createInstance)
     */
    void bind() const;

    static void setActiveTexture(GLint index);

    const GLuint _textureId;

};


#endif //JUMPERBALLAPPLICATION_TEXTURESAMPLER_H
