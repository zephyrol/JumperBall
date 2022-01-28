//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "BloomProcess.h"

BloomProcess::BloomProcess(
        GLsizei width,
        GLsizei height,
        const RenderPass_sptr& screen
):
        _width(width),
        _height(height),
        _screen(screen),
        _bloomBlurShader(ShaderProgram::createShaderProgram("basicFboVs.vs", "bloomFs.fs" ))
{
}

void BloomProcess::render() const {
    _bloomBlurShader->use();
    _screen->render(_bloomBlurShader);
}

void BloomProcess::freeGPUMemory() {
    _bloomBlurShader->freeGPUMemory();
}

std::shared_ptr<const GLuint> BloomProcess::getRenderTexture() const {
    return nullptr;
}

