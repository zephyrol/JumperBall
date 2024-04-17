//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LEVELPROCESS_H
#define JUMPERBALLAPPLICATION_LEVELPROCESS_H

#include "RenderingCache.h"
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
                 FrameBuffer_uptr firstShadow,
                 FrameBuffer_uptr firstBlankShadow,
                 FrameBuffer_uptr secondShadow,
                 FrameBuffer_uptr secondBlankShadow,
                 FrameBuffer_uptr levelFrameBuffer,
                 RenderGroup_sptr mapGroup,
                 ShaderProgram_uptr mapShaderProgram,
                 RenderGroup_sptr starGroup,
                 ShaderProgram_uptr starShaderProgram);

    void update() override;

    void render() const override;

    const CstTextureSampler_uptr& getRenderTexture() const override;

    GLsizeiptr getUniformBufferSize() const;
    std::vector<GLint> getUniformBufferFieldOffsets(const std::vector<std::string>& fieldNames) const;

    void fillCache(RenderingCache& renderingCache);

   private:
    static constexpr GLint firstShadowTextureIndex = 0;
    static constexpr GLint secondShadowTextureIndex = 1;
    static const GLsizei depthTexturesSize;
    static const std::string firstShadowHash;
    static const std::string secondShadowHash;
    static const std::string firstBlankShadowHash;
    static const std::string secondBlankShadowHash;


    const GLsizei _width;
    const GLsizei _height;

    FrameBuffer_uptr _firstShadow;
    FrameBuffer_uptr _firstBlankShadow;
    FrameBuffer_uptr _secondShadow;
    FrameBuffer_uptr _secondBlankShadow;
    FrameBuffer_uptr _levelFrameBuffer;

    const RenderGroup_sptr _mapGroup;
    ShaderProgram_uptr _mapShaderProgram;
    RenderGroupUniforms _mapGroupUniforms;

    const RenderGroup_sptr _starGroup;
    ShaderProgram_uptr _starShaderProgram;
    RenderGroupUniforms _starGroupUniforms;

    const GLint _passIdUniformLocation;
    const std::string _uniformBufferName;

    static ShaderProgram_uptr createMapShaderProgram(const JBTypes::FileContent& fileContent,
                                                     short idCount,
                                                     GLuint uniformBufferBindingPoint,
                                                     const std::string& uniformBufferName,
                                                     RenderingCache& renderingCache);

    static std::string getLevelFrameBufferHash(GLsizei width, GLsizei height);
};

#endif  // JUMPERBALLAPPLICATION_LEVELPROCESS_H
