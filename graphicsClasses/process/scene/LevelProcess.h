//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LEVELPROCESS_H
#define JUMPERBALLAPPLICATION_LEVELPROCESS_H

#include "RenderingCache.h"
#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/DepthFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"
#include "process/RenderGroup.h"
#include "process/RenderProcess.h"
#include "process/UniformBuffer.h"
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
                                            GLuint uniformBufferBindingPoint,
                                            const std::string& uniformBufferName,
                                            unsigned int ballSkin,
                                            RenderingCache& renderingCache);

    LevelProcess(GLsizei width,
                 GLsizei height,
                 std::string uniformBufferName,
                 DepthFrameBuffer_uptr firstShadow,
                 DepthFrameBuffer_uptr firstBlankShadow,
                 DepthFrameBuffer_uptr secondShadow,
                 DepthFrameBuffer_uptr secondBlankShadow,
                 ColorableFrameBuffer_uptr levelFrameBuffer,
                 RenderGroup_sptr mapGroup,
                 ShaderProgram_uptr mapShaderProgram,
                 RenderGroup_sptr starGroup,
                 ShaderProgram_uptr starShaderProgram);

    void update() override;

    void render() const override;

    const CstTextureSampler_uptr& getRenderTexture() const override;

    GLsizeiptr getUniformBufferSize() const;
    std::vector<GLint> getUniformBufferFieldOffsets(const std::vector<std::string>& fieldNames) const;

   private:
    static constexpr GLsizei depthTexturesSize = 1024;
    static constexpr GLint firstShadowTextureIndex = 0;
    static constexpr GLint secondShadowTextureIndex = 1;

    const GLsizei _width;
    const GLsizei _height;

    const DepthFrameBuffer_uptr _firstShadow;
    const DepthFrameBuffer_uptr _firstBlankShadow;
    const DepthFrameBuffer_uptr _secondShadow;
    const DepthFrameBuffer_uptr _secondBlankShadow;
    const ColorableFrameBuffer_uptr _levelFrameBuffer;

    const RenderGroup_sptr _mapGroup;
    const ShaderProgram_uptr _mapShaderProgram;
    RenderGroupUniforms _mapGroupUniforms;

    const RenderGroup_sptr _starGroup;
    const ShaderProgram_uptr _starShaderProgram;
    RenderGroupUniforms _starGroupUniforms;

    const GLint _passIdUniformLocation;
    const std::string _uniformBufferName;

    static ShaderProgram_uptr createMapShaderProgram(const JBTypes::FileContent& fileContent,
                                                     short idCount,
                                                     GLuint uniformBufferBindingPoint,
                                                     const std::string& uniformBufferName,
                                                     RenderingCache& renderingCache);
};

#endif  // JUMPERBALLAPPLICATION_LEVELPROCESS_H
