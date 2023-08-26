//
// Created by S.Morgenthaler on 21/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_POSTEFFECTS_H
#define JUMPERBALLAPPLICATION_POSTEFFECTS_H

#include "process/RenderProcess.h"
#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"
#include "process/RenderGroup.h"

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

    ShaderProgram_sptr createPostProcessesShaderProgram(
        GLuint sceneTexture,
        const JBTypes::FileContent &fileContent,
        GLsizei width,
        GLsizei height
    );

    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:

    static constexpr GLint postProcessTextureNumber = 3;

    const GLsizei _screenWidth;
    const GLsizei _screenHeight;
    const GLsizei _postEffectsWidth;
    const GLsizei _postEffectsHeight;
    const CstRenderGroup_sptr _screen;
    const ColorableFrameBuffer_uptr _brightPassFilterFrameBuffer;
    const ColorableFrameBuffer_uptr _horizontalBlurFrameBuffer;
    const ColorableFrameBuffer_uptr _verticalBlurFrameBuffer;
    const ShaderProgram_sptr _postProcessesShader;
    const GLint _postProcessIdUniformLocation;
    const GLint _defaultFrameBuffer;
};


#endif //JUMPERBALLAPPLICATION_POSTEFFECTS_H
