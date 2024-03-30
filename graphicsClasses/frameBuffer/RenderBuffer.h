//
// Created by S.Morgenthaler on 30/03/2024.
//

#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H
#include "Utility.h"

class RenderBuffer;

using CstRenderBuffer_uptr = std::unique_ptr<const RenderBuffer>;
using vecCstRenderBuffer_uptr = std::vector<CstRenderBuffer_uptr>;

class RenderBuffer {
public:
    RenderBuffer();

    /**
     * Copy constructor and assignment are deleted because a OpenGL buffer id is unique.
     */
    RenderBuffer(const RenderBuffer &renderBuffer) = delete;
    RenderBuffer &operator=(const RenderBuffer &renderBuffer) = delete;

    /**
     * Move constructor and assignment are deleted because the destructor free the GPU memory.
     */
    RenderBuffer(RenderBuffer &&renderBuffer) = delete;
    RenderBuffer &operator=(RenderBuffer &&renderBuffer) = delete;

    /**
     * Bind a texture from its id.
     */
    void bind() const;

    /**
    * Get OpenGL buffer Id. Use this getter ONLY to reference the ID to OpenGL API.
    * Do not store it anywhere!
    */
    GLuint getId() const;

    ~RenderBuffer();

private:
    const GLuint _renderBufferId;
};



#endif //RENDERBUFFER_H
