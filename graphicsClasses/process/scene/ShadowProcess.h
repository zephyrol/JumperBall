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

    static ShadowProcess_sptr createInstance(
        const JBTypes::FileContent &fileContent,
        RenderPass_sptr blocks,
        RenderPass_sptr items,
        RenderPass_sptr enemies,
        RenderPass_sptr specials,
        RenderPass_sptr ball,
        bool isFirst
    );

    ShadowProcess(
        DepthFrameBuffer_uptr frameBuffer,
        std::vector<std::pair<CstShaderProgram_sptr, RenderPass_sptr> >&& shadersRenderPasses,
        bool isFirst
    );

    void render() const override;

    void freeGPUMemory() override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    std::shared_ptr<const GLuint> getRenderTexture() const override;

    ~ShadowProcess() override = default;

private:
    static constexpr size_t sizeDepthTexture = 1024;
    const DepthFrameBuffer_uptr _frameBuffer;
    const std::vector<std::pair<CstShaderProgram_sptr, RenderPass_sptr> > _shadersRenderPasses;
    const bool _isFirst;
};

#endif //JUMPERBALLAPPLICATION_SHADOWPROCESS_H
