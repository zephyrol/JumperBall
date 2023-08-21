//
// Created by S.Morgenthaler on 21/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_POSTPROCESSES_H
#define JUMPERBALLAPPLICATION_POSTPROCESSES_H

#include "process/RenderProcess.h"
#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"

class PostProcesses: public RenderProcess {

public:
    PostProcesses(
        const JBTypes::FileContent &fileContent,
        GLsizei width,
        GLsizei height,
        GLuint sceneTexture,
        GLint defaultFrameBuffer,
        const CstRenderGroup_sptr &screen
    );

    void render() const override;

    void freeGPUMemory() override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    static ShaderProgram_sptr createPostProcessesShaderProgram(
        const JBTypes::FileContent &fileContent,
        GLsizei width,
        GLsizei height
    );

    static ColorableFrameBuffer_uptr createPostProcessFrameBuffer(
        GLsizei width,
        GLsizei height
    );

private:
    const GLsizei _width;
    const GLsizei _height;
    const ShaderProgram_sptr _postProcessesShader;
    RenderPass _screenRenderPass;
    const ColorableFrameBuffer_uptr _frameBufferA;
    const ColorableFrameBuffer_uptr _frameBufferB;
    const GLuint _sceneTexture;
    const GLint _defaultFrameBuffer;
};


#endif //JUMPERBALLAPPLICATION_POSTPROCESSES_H
