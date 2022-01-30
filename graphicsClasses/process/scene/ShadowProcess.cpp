//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "ShadowProcess.h"

ShadowProcess::ShadowProcess(
    const RenderPass_sptr& blocks,
    const RenderPass_sptr& items,
    const RenderPass_sptr& enemies,
    const RenderPass_sptr& specials,
    const RenderPass_sptr& ball
): _frameBuffer(
        new FrameBuffer(
                sizeDepthTexture,
                sizeDepthTexture,
                FrameBuffer::Content::Depth,
                true,
                true,
                { 1.f, 1.f, 1.f }
        )),
        _blocks(blocks),
        _items(items),
        _enemies(enemies),
        _specials(specials),
        _ball(ball),
        _shadowBlocksShader(ShaderProgram::createShaderProgram("blocksVs.vs", depthFs)),
        _shadowItemsShader(ShaderProgram::createShaderProgram("itemsMapVs.vs", depthFs)),
        _shadowEnemiesShader(ShaderProgram::createShaderProgram("enemiesVs.vs", depthFs)),
        _shadowSpecialsShader(ShaderProgram::createShaderProgram("specialsVs.vs", depthFs)),
        _shadowBallShader(ShaderProgram::createShaderProgram("ballVs.vs", depthFs))
{
}

void ShadowProcess::render() const {

    _frameBuffer->bindFrameBuffer();

    FrameBuffer::setViewportSize(sizeDepthTexture, sizeDepthTexture);
    FrameBuffer::enableDepthTest();

    _shadowBlocksShader->use();
    _blocks->render(_shadowBlocksShader);

    _shadowItemsShader->use();
    _items->render(_shadowItemsShader);

    _shadowEnemiesShader->use();
    _enemies->render(_shadowEnemiesShader);

    _shadowSpecialsShader->use();
    _specials->render(_shadowSpecialsShader);

    _shadowBallShader->use();
    _ball->render(_shadowBallShader);
}

void ShadowProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _shadowBlocksShader->freeGPUMemory();
    _shadowEnemiesShader->freeGPUMemory();
    _shadowItemsShader->freeGPUMemory();
    _shadowSpecialsShader->freeGPUMemory();
    _shadowBallShader->freeGPUMemory();
}

const std::string ShadowProcess::depthFs = "depthFs.fs";

std::shared_ptr<const GLuint> ShadowProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

vecCstShaderProgram_sptr ShadowProcess::getShaderPrograms() const {
    return {
        _shadowBlocksShader,
        _shadowItemsShader,
        _shadowEnemiesShader,
        _shadowSpecialsShader,
        _shadowBallShader
    };
}
