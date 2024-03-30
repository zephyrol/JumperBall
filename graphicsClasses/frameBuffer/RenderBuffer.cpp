//
// Created by S.Morgenthaler on 30/03/2024.
//

#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(): _renderBufferId(
    []() {
        GLuint renderBuffer;
        glGenRenderbuffers(1, &renderBuffer);
        return renderBuffer;
    }()
) {
}

void RenderBuffer::bind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferId);
}

GLuint RenderBuffer::getId() const {
    return _renderBufferId;
}

RenderBuffer::~RenderBuffer() {
    glDeleteRenderbuffers(1, &_renderBufferId);
}
