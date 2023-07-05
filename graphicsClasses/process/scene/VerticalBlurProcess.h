//
// Created by S.Morgenthaler on 26/01/2022.
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
        const CstRenderGroupsManager_sptr &screen
    );

    void render() const override;

    void freeGPUMemory() override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    const ShaderProgram_sptr _verticalBlurShader;
    RenderPass _screenRenderPass;
    const ColorableFrameBuffer_uptr _frameBuffer;
    const GLuint _horizontalBlurTexture;

    static ShaderProgram_sptr createVerticalBlurProcessShaderProgram(
        const JBTypes::FileContent &fileContent,
        GLsizei height
    );
};


#endif //JUMPERBALLAPPLICATION_VERTICALBLURPROCESS_H
