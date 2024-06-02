//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LEVELPROCESS_H
#define JUMPERBALLAPPLICATION_LEVELPROCESS_H

#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/DepthFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"
#include "process/RenderGroup.h"
#include "process/RenderProcess.h"
#include "scene/Star.h"

class LevelProcess;

using LevelProcess_uptr = std::unique_ptr<LevelProcess>;

class LevelProcess : public RenderProcess {
   public:
    static LevelProcess_uptr createInstance(const JBTypes::FileContent& fileContent,
                                            GLsizei width,
                                            GLsizei height,
                                            CstMap_sptr map,
                                            CstStar_sptr firstStar,
                                            CstStar_sptr secondStar,
                                            unsigned int ballSkin);

    LevelProcess(GLsizei width,
                 GLsizei height,
                 DepthFrameBuffer_uptr firstShadow,
                 DepthFrameBuffer_uptr secondShadow,
                 ColorableFrameBuffer_uptr levelFrameBuffer,
                 CstTextureSampler_uptr depthKernel,
                 RenderGroup_sptr mapGroup,
                 ShaderProgram_sptr mapShaderProgram,
                 RenderGroup_sptr starGroup,
                 ShaderProgram_sptr starShaderProgram
                 );

    void update() override;

    void render() const override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    const CstTextureSampler_uptr& getRenderTexture() const override;

    static constexpr GLint levelTextureIndex = 3;

   private:
    static constexpr GLsizei depthTexturesSize = 1024;
    static constexpr GLsizei kernelTextureSize = 8;
    static constexpr GLint firstDepthTextureIndex = 0;
    static constexpr GLint secondDepthTextureIndex = 1;
    static constexpr GLint kernelTextureIndex = 2;

    const GLsizei _width;
    const GLsizei _height;

    const DepthFrameBuffer_uptr _firstShadow;
    const DepthFrameBuffer_uptr _secondShadow;
    const ColorableFrameBuffer_uptr _levelFrameBuffer;
    const CstTextureSampler_uptr _depthKernel;

    const RenderGroup_sptr _mapGroup;
    const ShaderProgram_sptr _mapShaderProgram;
    RenderGroupUniforms _mapGroupUniforms;

    const RenderGroup_sptr _starGroup;
    const ShaderProgram_sptr _starShaderProgram;
    RenderGroupUniforms _starGroupUniforms;

    const GLint _passIdUniformLocation;
    const GLint _depthTextureUniformLocation;
    const GLint _depth2TextureUniformLocation;

    static ShaderProgram_sptr createMapShaderProgram(const JBTypes::FileContent& fileContent,
                                                     short idCount,
                                                     GLsizei width,
                                                     GLsizei height);

    static CstTextureSampler_uptr createDepthKernel();
};

#endif  // JUMPERBALLAPPLICATION_LEVELPROCESS_H
