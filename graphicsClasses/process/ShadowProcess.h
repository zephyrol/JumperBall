//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_SHADOWPROCESS_H
#define JUMPERBALLAPPLICATION_SHADOWPROCESS_H
#include "RenderProcess.h"


class ShadowProcess: public RenderProcess {
public:

    ShadowProcess(
        const RenderPass_sptr& blocks,
        const RenderPass_sptr& items,
        const RenderPass_sptr& enemies,
        const RenderPass_sptr& specials,
        const RenderPass_sptr& ball
    );

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    static constexpr size_t sizeDepthTexture = 1024;

    const FrameBuffer_uptr _frameBuffer;

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
};

#endif //JUMPERBALLAPPLICATION_SHADOWPROCESS_H
