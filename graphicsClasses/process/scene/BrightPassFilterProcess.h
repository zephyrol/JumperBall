//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_BRIGHTPASSFILTERPROCESS_H
#define JUMPERBALLAPPLICATION_BRIGHTPASSFILTERPROCESS_H


#include "process/RenderProcess.h"

class BrightPassFilterProcess: RenderProcess {

public:
    BrightPassFilterProcess(
        GLsizei width,
        GLsizei height,
        GLuint hdrSceneTexture,
        const RenderPass_sptr& screen
    );

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;
    CstShaderProgram_sptr createBrightPassFilterProcessShaderProgram() const;

private:
    const RenderPass_sptr _screen;
    const FrameBuffer_uptr _frameBuffer;
    const GLuint _hdrSceneTexture;
    const CstShaderProgram_sptr _brightPassFilterShader;
};


#endif //JUMPERBALLAPPLICATION_BRIGHTPASSFILTERPROCESS_H
