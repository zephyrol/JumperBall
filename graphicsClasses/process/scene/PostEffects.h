//
// Created by S.Morgenthaler on 21/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_POSTEFFECTS_H
#define JUMPERBALLAPPLICATION_POSTEFFECTS_H

#include "frameBuffer/ColorableFrameBuffer.h"
#include "process/RenderGroup.h"

class PostEffects;
using PostEffects_uptr = std::unique_ptr<PostEffects>;
class PostEffects {
   public:
    PostEffects(

        GLsizei screenWidth,
        GLsizei screenHeight,
        GLsizei postEffectsWidth,
        GLsizei postEffectsHeight,
        RenderGroup_sptr screen,
        ColorableFrameBuffer_uptr brightPassFilterFrameBuffer,
        ColorableFrameBuffer_uptr horizontalBlurFrameBuffer,
        ColorableFrameBuffer_uptr verticalBlurFrameBuffer,
        ShaderProgram_sptr postProcessesShader,
        GLint postProcessIdUniformLocation,
        GLint postProcessTextureUniformLocation,
        GLint defaultFrameBuffer);

    void render() const;

    static PostEffects_uptr createInstance(const JBTypes::FileContent& fileContent,
                                    GLsizei screenWidth,
                                    GLsizei screenHeight,
                                    GLsizei postEffectsWidth,
                                    GLsizei postEffectsHeight,
                                    GLint sceneTextureIndex,
                                    GLint defaultFrameBuffer);

    vecCstShaderProgram_sptr getShaderPrograms() const;

   private:
    static constexpr GLint brightPassFilterTextureIndex = 4;
    static constexpr GLint horizontalBlurTextureIndex = 5;
    static constexpr GLint verticalBlurTextureIndex = 6;

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
    const GLint _postProcessTextureUniformLocation;
    const GLint _defaultFrameBuffer;
};

#endif  // JUMPERBALLAPPLICATION_POSTEFFECTS_H
