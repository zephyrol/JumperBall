//
// Created by S.Morgenthaler on 03/02/2023.
//

#ifndef JUMPERBALLAPPLICATION_TEXTURESAMPLER_H
#define JUMPERBALLAPPLICATION_TEXTURESAMPLER_H
#include "ShaderProgram.h"


namespace TextureSampler {

    /**
     * Bind a texture from its id.
     * Warning: Ensure that TextureSampler::setActiveTexture was previously called with
     * the index corresponding to one was used during the texture registration in shader program.
     * (using ShaderProgram::setTextureIndex)
     */
    void bind(GLuint textureId);

    /**
     * Set the current active texture.
     * After this call, the TextureSampler.bind() function will bind a texture id to
     * this specified index.
     */
    void setActiveTexture(GLint index);

}


#endif //JUMPERBALLAPPLICATION_TEXTURESAMPLER_H
