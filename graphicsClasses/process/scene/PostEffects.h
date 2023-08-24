//
// Created by S.Morgenthaler on 21/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_POSTEFFECTS_H
#define JUMPERBALLAPPLICATION_POSTEFFECTS_H

#include "process/RenderProcess.h"
#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"

class PostEffects : public RenderProcess {

public:
    PostEffects(
        const JBTypes::FileContent &fileContent,
        GLsizei screenWidth,
        GLsizei screenHeight,
        GLsizei postEffectsWidth,
        GLsizei postEffectsHeight,
        GLuint sceneTexture,
        GLint defaultFrameBuffer
    );

    void render() const override;

    void freeGPUMemory() override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    static ShaderProgram_sptr createPostProcessesShaderProgram(
        const JBTypes::FileContent &fileContent,
        GLsizei width,
        GLsizei height
    );

    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:

    const GLsizei _screenWidth;
    const GLsizei _screenHeight;
    const GLsizei _postEffectsWidth;
    const GLsizei _postEffectsHeight;
    const ShaderProgram_sptr _postProcessesShader;
    const GLint _postProcessIdUniformLocation;
    const CstRenderGroup_sptr _screen;
    const ColorableFrameBuffer_uptr _brightPassFilterFrameBuffer;
    const ColorableFrameBuffer_uptr _horizontalBlurFrameBuffer;
    const ColorableFrameBuffer_uptr _verticalBlurFrameBuffer;
    const GLuint _sceneTexture;
    const GLint _defaultFrameBuffer;
};


#endif //JUMPERBALLAPPLICATION_POSTEFFECTS_H
