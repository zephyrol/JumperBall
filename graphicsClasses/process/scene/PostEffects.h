//
// Created by S.Morgenthaler on 21/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_POSTEFFECTS_H
#define JUMPERBALLAPPLICATION_POSTEFFECTS_H

#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"
#include "process/RenderGroup.h"

class PostEffects;
using PostEffects_uptr = std::unique_ptr<PostEffects>;
class PostEffects {

public:
    PostEffects(
        const JBTypes::FileContent &fileContent,
        GLsizei screenWidth,
        GLsizei screenHeight,
        GLsizei postEffectsWidth,
        GLsizei postEffectsHeight,
        const CstTextureSampler_uptr& sceneTexture,
        GLint defaultFrameBuffer
    );

    void render() const;

    vecCstShaderProgram_sptr getShaderPrograms() const;

    ShaderProgram_sptr createPostProcessesShaderProgram(
        const CstTextureSampler_uptr &sceneTexture,
        const JBTypes::FileContent &fileContent,
        GLsizei width,
        GLsizei height
    );

private:

    static constexpr GLint postProcessTextureNumber = 4;

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
