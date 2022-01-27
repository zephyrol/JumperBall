//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_VERTICALBLURPROCESS_H
#define JUMPERBALLAPPLICATION_VERTICALBLURPROCESS_H


#include "RenderProcess.h"

class VerticalBlurProcess: RenderProcess {

public:
    VerticalBlurProcess(GLsizei width, GLsizei height, const RenderPass_sptr& screen);

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    const RenderPass_sptr _screen;
    const FrameBuffer_uptr _frameBuffer;
    const CstShaderProgram_sptr _bloomShader;
};


#endif //JUMPERBALLAPPLICATION_VERTICALBLURPROCESS_H
