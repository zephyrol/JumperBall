//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_SHADOWPROCESS_H
#define JUMPERBALLAPPLICATION_SHADOWPROCESS_H

#include "process/RenderProcess.h"
#include "frameBuffer/DepthFrameBuffer.h"

class ShadowProcess;
using ShadowProcess_sptr = std::shared_ptr<ShadowProcess>;

class ShadowProcess : public RenderProcess {
public:

    static ShadowProcess_sptr createInstance(const JBTypes::FileContent &fileContent, CstRenderGroup_sptr map, bool isFirst);

    ShadowProcess(
        DepthFrameBuffer_uptr frameBuffer,
        ShaderProgram_sptr shaderProgram,
        RenderPass_sptr renderPass,
        bool isFirst,
        GLsizei depthTextureSize
    );

    void update() override;

    void render() const override;

    void freeGPUMemory() override;

    GLsizei depthTextureSize() const;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    std::shared_ptr<const GLuint> getRenderTexture() const override;

    ~ShadowProcess() override = default;

private:
    const DepthFrameBuffer_uptr _frameBuffer;
    const ShaderProgram_sptr _shaderProgram;
    const RenderPass_sptr _renderPass;
    const bool _isFirst;
    const GLsizei _depthTextureSize;
};

#endif //JUMPERBALLAPPLICATION_SHADOWPROCESS_H
