//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "ShadowProcess.h"

#include <utility>

ShadowProcess::ShadowProcess(
    const JBTypes::FileContent &fileContent,
    RenderPass_sptr blocks,
    RenderPass_sptr items,
    RenderPass_sptr enemies,
    RenderPass_sptr specials,
    RenderPass_sptr ball,
    bool isFirst
) : _frameBuffer(DepthFrameBuffer::createInstance(
        sizeDepthTexture,
        sizeDepthTexture
    )),
    _isFirst(isFirst),
    _blocks(std::move(blocks)),
    _items(std::move(items)),
    _enemies(std::move(enemies)),
    _specials(std::move(specials)),
    _ball(std::move(ball)),
    _shadowBlocksShader(ShaderProgram::createInstance(
        fileContent,
        "blocksVs.vs",
        depthFs,
        _blocks->genUniformNames(),
        getShadowDefines()
    )),
    _shadowItemsShader(ShaderProgram::createInstance(
        fileContent,
        "itemsMapVs.vs",
        depthFs,
        _items->genUniformNames(),
        getShadowDefines()
    )),
    _shadowEnemiesShader(ShaderProgram::createInstance(
        fileContent,
        "enemiesVs.vs",
        depthFs,
        _enemies->genUniformNames(),
        getShadowDefines()
    )),
    _shadowSpecialsShader(ShaderProgram::createInstance(
        fileContent,
        "specialsVs.vs",
        depthFs,
        _specials->genUniformNames(),
        getShadowDefines()
    )),
    _shadowBallShader(ShaderProgram::createInstance(
        fileContent,
        "ballVs.vs",
        depthFs,
        _ball->genUniformNames(),
        getShadowDefines()
    )) {
}

void ShadowProcess::render() const {

    if (_isFirst) {
        glCullFace(GL_FRONT);
        FrameBuffer::enableDepthTest();
        FrameBuffer::setViewportSize(sizeDepthTexture, sizeDepthTexture);
    }
    _frameBuffer->bindFrameBuffer();
    _frameBuffer->clear();

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
const std::vector<std::string> ShadowProcess::shadowDefines = {"SHADOW_PASS"};
const std::vector<std::string> ShadowProcess::shadow2Defines = {"SHADOW_PASS_2"};

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

std::vector<std::string> ShadowProcess::getShadowDefines() const {
    return _isFirst ? std::vector<std::string>{"SHADOW_PASS"} : std::vector<std::string>{"SHADOW_PASS_2"};
}
