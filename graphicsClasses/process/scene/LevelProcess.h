//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LEVELPROCESS_H
#define JUMPERBALLAPPLICATION_LEVELPROCESS_H

#include "process/RenderProcess.h"
#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"
#include "frameBuffer/DepthFrameBuffer.h"
#include "scene/Star.h"
#include "process/RenderGroup.h"


class LevelProcess;

using LevelProcess_sptr = std::shared_ptr<LevelProcess>;

class LevelProcess : public RenderProcess {
public:

    static LevelProcess_sptr createInstance(
        const JBTypes::FileContent &fileContent,
        GLsizei width,
        GLsizei height,
        CstMap_sptr map,
        CstStar_sptr firstStar,
        CstStar_sptr secondStar,
        unsigned int ballSkin
    );

    LevelProcess(
        GLsizei width,
        GLsizei height,
        DepthFrameBuffer_uptr firstShadow,
        DepthFrameBuffer_uptr firstBlankShadow,
        DepthFrameBuffer_uptr secondShadow,
        DepthFrameBuffer_uptr secondBlankShadow,
        ColorableFrameBuffer_uptr levelFrameBuffer,
        RenderGroup_sptr mapGroup,
        ShaderProgram_sptr mapShaderProgram,
        RenderGroup_sptr starGroup,
        ShaderProgram_sptr starShaderProgram
    );

    void update() override;

    void render() const override;

    void freeGPUMemory() override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:

    static constexpr GLsizei depthTexturesSize = 1024;

    const GLsizei _width;
    const GLsizei _height;

    const DepthFrameBuffer_uptr _firstShadow;
    const DepthFrameBuffer_uptr _firstBlankShadow;
    const DepthFrameBuffer_uptr _secondShadow;
    const DepthFrameBuffer_uptr _secondBlankShadow;
    const ColorableFrameBuffer_uptr _levelFrameBuffer;

    const RenderGroup_sptr _mapGroup;
    const ShaderProgram_sptr _mapShaderProgram;
    RenderGroupUniforms _mapGroupUniforms;

    const RenderGroup_sptr _starGroup;
    const ShaderProgram_sptr _starShaderProgram;
    RenderGroupUniforms _starGroupUniforms;

    const GLint _passIdUniformLocation;


    static ShaderProgram_sptr createMapShaderProgram(
        const JBTypes::FileContent &fileContent,
        short idCount
    );
};

#endif //JUMPERBALLAPPLICATION_LEVELPROCESS_H
