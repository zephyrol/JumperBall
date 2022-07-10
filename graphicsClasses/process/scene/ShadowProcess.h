//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_SHADOWPROCESS_H
#define JUMPERBALLAPPLICATION_SHADOWPROCESS_H

#include "process/RenderProcess.h"


class ShadowProcess : public RenderProcess {
public:

    ShadowProcess(
        const JBTypes::FileContent &fileContent,
        const RenderPass_sptr &blocks,
        const RenderPass_sptr &items,
        const RenderPass_sptr &enemies,
        const RenderPass_sptr &specials,
        const RenderPass_sptr &ball,
        bool isFirst
    );

    void render() const override;

    void freeGPUMemory() override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    std::shared_ptr<const GLuint> getRenderTexture() const override;

    virtual ~ShadowProcess() = default;

private:
    static constexpr size_t sizeDepthTexture = 1024;

    const std::vector<std::string> &getShadowDefines() const;

    const FrameBuffer_uptr _frameBuffer;
    const bool _isFirst;

    const RenderPass_sptr _blocks;
    const RenderPass_sptr _items;
    const RenderPass_sptr _enemies;
    const RenderPass_sptr _specials;
    const RenderPass_sptr _ball;
    const CstShaderProgram_sptr _shadowBlocksShader;
    const CstShaderProgram_sptr _shadowItemsShader;
    const CstShaderProgram_sptr _shadowEnemiesShader;
    const CstShaderProgram_sptr _shadowSpecialsShader;
    const CstShaderProgram_sptr _shadowBallShader;

    static const std::string depthFs;
    static const std::vector<std::string> shadowDefines;
    static const std::vector<std::string> shadow2Defines;
};

#endif //JUMPERBALLAPPLICATION_SHADOWPROCESS_H
