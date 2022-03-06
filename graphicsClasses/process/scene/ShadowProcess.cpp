//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "ShadowProcess.h"

ShadowProcess::ShadowProcess(
    const JBTypes::FileContent& fileContent,
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
                { 1.f, 1.f, 1.f }
        )),
        _blocks(blocks),
        _items(items),
        _enemies(enemies),
        _specials(specials),
        _ball(ball),
        _shadowBlocksShader(ShaderProgram::createShaderProgram(
            fileContent,
            "blocksVs.vs",
            depthFs,
            shadowDefines
        )),
        _shadowItemsShader(ShaderProgram::createShaderProgram(
            fileContent,
            "itemsMapVs.vs",
            depthFs,
            shadowDefines
        )),
        _shadowEnemiesShader(ShaderProgram::createShaderProgram(
            fileContent,
            "enemiesVs.vs",
            depthFs,
            shadowDefines
        )),
        _shadowSpecialsShader(ShaderProgram::createShaderProgram(
            fileContent,
            "specialsVs.vs",
            depthFs,
            shadowDefines
        )),
        _shadowBallShader(ShaderProgram::createShaderProgram(
            fileContent,
            "ballVs.vs",
            depthFs,
            shadowDefines
        ))
{
}

void ShadowProcess::render() const {

    FrameBuffer::enableDepthTest();
    FrameBuffer::setViewportSize(sizeDepthTexture, sizeDepthTexture);
    _frameBuffer->bindFrameBuffer();
    _frameBuffer->clean();

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
const std::vector<std::string> ShadowProcess::shadowDefines= { "SHADOW_PASS" };

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
