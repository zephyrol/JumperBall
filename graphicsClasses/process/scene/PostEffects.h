//
// Created by S.Morgenthaler on 21/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_POSTEFFECTS_H
#define JUMPERBALLAPPLICATION_POSTEFFECTS_H

#include "RenderingCache.h"
#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"
#include "process/RenderGroup.h"

class PostEffects;
using PostEffects_uptr = std::unique_ptr<PostEffects>;
class PostEffects {
   public:
    PostEffects(const JBTypes::FileContent& fileContent,
                GLsizei screenWidth,
                GLsizei screenHeight,
                GLsizei postEffectsWidth,
                GLsizei postEffectsHeight,
                GLuint uniformBufferBindingPoint,
                const std::string& uniformBufferName,
                const CstTextureSampler_uptr& sceneTexture,
                GLint defaultFrameBuffer,
                RenderingCache& renderingCache);

    void render() const;

    ShaderProgram_uptr createPostProcessesShaderProgram(const CstTextureSampler_uptr& sceneTexture,
                                                        const JBTypes::FileContent& fileContent,
                                                        GLsizei width,
                                                        GLsizei height,
                                                        GLuint uniformBufferBindingPoint,
                                                        const std::string& uniformBufferName,
                                                        RenderingCache& renderingCache);

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
    const ShaderProgram_uptr _postProcessesShader;
    const GLint _postProcessIdUniformLocation;
    const GLint _defaultFrameBuffer;
};

#endif  // JUMPERBALLAPPLICATION_POSTEFFECTS_H
