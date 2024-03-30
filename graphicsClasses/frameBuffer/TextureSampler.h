//
// Created by S.Morgenthaler on 03/02/2023.
//

#ifndef JUMPERBALLAPPLICATION_TEXTURESAMPLER_H
#define JUMPERBALLAPPLICATION_TEXTURESAMPLER_H
#include "ShaderProgram.h"


class TextureSampler;
using CstTextureSampler_sptr = std::shared_ptr<const TextureSampler>;
using CstTextureSampler_uptr = std::unique_ptr<const TextureSampler>;

class TextureSampler {
public :
    TextureSampler();

    /**
     * Copy constructor and assignment are deleted because a OpenGL texture id is unique.
     */
    TextureSampler(const TextureSampler& textureSampler) = delete;
    TextureSampler& operator=(const TextureSampler& textureSampler) = delete;

    /**
     * Move constructor and assignment are deleted because the destructor free the GPU memory.
     */
    TextureSampler(TextureSampler&& textureSampler) = delete;
    TextureSampler& operator=(TextureSampler&& textureSampler) = delete;

    /**
     * Bind a texture from its id.
     * Warning: Ensure that TextureSampler::setActiveTexture was previously called with
     * the index corresponding to one was used during the texture registration in shader program.
     * (using ShaderProgram::setTextureIndex)
     */
    void bind() const;

    /**
     * Set the current active texture.
     * After this call, the TextureSampler.bind() function will bind a texture id to
     * this specified index.
     */
    static void setActiveTexture(GLint index);

    /**
    * Get OpenGL texture Id. Use this getter ONLY to reference the ID to OpenGL API.
    * Do not store it anywhere!
    */
    GLuint getId() const;

    ~TextureSampler();

private:
    const GLuint _textureId;
};


#endif //JUMPERBALLAPPLICATION_TEXTURESAMPLER_H
