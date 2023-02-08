//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_VERTICALBLURPROCESS_H
#define JUMPERBALLAPPLICATION_VERTICALBLURPROCESS_H


#include "process/RenderProcess.h"
#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"

class VerticalBlurProcess : public RenderProcess {

public:
    VerticalBlurProcess(
        const JBTypes::FileContent &fileContent,
        GLsizei width,
        GLsizei height,
        GLuint horizontalBlurTexture,
        RenderPass_sptr screen
    );

    void render() const override;

    void freeGPUMemory() override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    const RenderPass_sptr _screen;
    const ColorableFrameBuffer_uptr _frameBuffer;
    const CstShaderProgram_sptr _verticalBlurShader;
    const TextureSampler _horizontalBlurTextureSampler;

    static CstShaderProgram_sptr createVerticalBlurProcessShaderProgram(
        const JBTypes::FileContent &fileContent,
        GLsizei width,
        GLsizei height
    );
};


#endif //JUMPERBALLAPPLICATION_VERTICALBLURPROCESS_H
