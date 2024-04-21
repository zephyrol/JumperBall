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
    PostEffects(GLsizei screenWidth,
                GLsizei screenHeight,
                GLsizei postEffectsWidth,
                GLsizei postEffectsHeight,
                FrameBuffer_uptr brightPassFilterFrameBuffer,
                FrameBuffer_uptr horizontalBlurFrameBuffer,
                FrameBuffer_uptr verticalBlurFrameBuffer,
                ShaderProgram_uptr postProcessesShader,
                GLint defaultFrameBuffer);

    void render() const;

    static PostEffects_uptr createInstance(const JBTypes::FileContent& fileContent,
                                           GLsizei screenWidth,
                                           GLsizei screenHeight,
                                           GLsizei postEffectsWidth,
                                           GLsizei postEffectsHeight,
                                           GLuint uniformBufferBindingPoint,
                                           const std::string& uniformBufferName,
                                           const CstTextureSampler_uptr& sceneTexture,
                                           GLint defaultFrameBuffer,
                                           RenderingCache& renderingCache);

    void fillCache(RenderingCache& renderingCache);

   private:
    static constexpr GLint postProcessTextureNumber = 3;
    static const std::string brightPassFilterFrameBufferHashBase;
    static const std::string horizontalBlurFrameBufferHashBase;
    static const std::string verticalBlurFrameBufferHashBase;

    static std::string getFrameBufferHash(const std::string& base, GLsizei width, GLsizei height);

    static ShaderProgram_uptr createPostProcessesShaderProgram(
        const CstTextureSampler_uptr& sceneTexture,
        const JBTypes::FileContent& fileContent,
        GLsizei width,
        GLsizei height,
        GLuint uniformBufferBindingPoint,
        const std::string& uniformBufferName,
        ColorableFrameBuffer_uptr const& verticalBlurFrameBuffer,
        RenderingCache& renderingCache);

    const GLsizei _screenWidth;
    const GLsizei _screenHeight;
    const GLsizei _postEffectsWidth;
    const GLsizei _postEffectsHeight;
    const CstRenderGroup_sptr _screen;
    FrameBuffer_uptr _brightPassFilterFrameBuffer;
    FrameBuffer_uptr _horizontalBlurFrameBuffer;
    FrameBuffer_uptr _verticalBlurFrameBuffer;
    ShaderProgram_uptr _postProcessesShader;
    const GLint _postProcessIdUniformLocation;
    const GLint _defaultFrameBuffer;
};

#endif  // JUMPERBALLAPPLICATION_POSTEFFECTS_H
