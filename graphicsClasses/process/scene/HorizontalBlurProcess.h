//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_HORIZONTALBLURPROCESS_H
#define JUMPERBALLAPPLICATION_HORIZONTALBLURPROCESS_H


#include "process/RenderProcess.h"

class HorizontalBlurProcess: public RenderProcess {

public:
    HorizontalBlurProcess(
        GLsizei width,
        GLsizei height,
        GLuint brightPassTexture,
        const RenderPass_sptr& screen
    );

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;
    vecCstShaderProgram_sptr getShaderPrograms() const override;
    static CstShaderProgram_sptr createHorizontalBlurProcessShaderProgram() ;

private:
    const RenderPass_sptr _screen;
    const FrameBuffer_uptr _frameBuffer;
    const GLuint _brightPassTexture;
    const CstShaderProgram_sptr _horizontalBlurShader;
};


#endif //JUMPERBALLAPPLICATION_HORIZONTALBLURPROCESS_H
