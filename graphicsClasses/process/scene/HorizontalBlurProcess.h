//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_HORIZONTALBLURPROCESS_H
#define JUMPERBALLAPPLICATION_HORIZONTALBLURPROCESS_H


#include "process/RenderProcess.h"
#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"

class HorizontalBlurProcess: public RenderProcess {

public:
    HorizontalBlurProcess(
        const JBTypes::FileContent& fileContent,
        GLsizei width,
        GLsizei height,
        GLuint brightPassTexture,
        const RenderGroupsManager_sptr& screen
    );

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;
    vecCstShaderProgram_sptr getShaderPrograms() const override;
    static ShaderProgram_sptr createHorizontalBlurProcessShaderProgram(
        const JBTypes::FileContent& fileContent,
        GLsizei width,
        GLsizei height
    );

private:
    const ShaderProgram_sptr _horizontalBlurShader;
    RenderPass _screenRenderPass;
    const ColorableFrameBuffer_uptr _frameBuffer;
    const GLuint _brightPassTexture;
};


#endif //JUMPERBALLAPPLICATION_HORIZONTALBLURPROCESS_H
