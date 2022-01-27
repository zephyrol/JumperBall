//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "LevelProcess.h"

LevelProcess::LevelProcess(
    GLsizei width,
    GLsizei height,
    const RenderPass_sptr& blocks,
    const RenderPass_sptr& items,
    const RenderPass_sptr& enemies,
    const RenderPass_sptr& specials,
    const RenderPass_sptr& ball,
    const RenderPass_sptr& star
): _frameBuffer(
        new FrameBuffer(
                width,
                height,
                FrameBuffer::Content::HDR,
                true
        )),
        _blocks(blocks),
        _items(items),
        _enemies(enemies),
        _specials(specials),
        _ball(ball),
        _star(star),
        _sceneBlocksShader(ShaderProgram::createShaderProgram("blocksVs.vs", levelFs)),
        _sceneItemsShader(ShaderProgram::createShaderProgram("itemsMapVs.vs", levelFs)),
        _sceneEnemiesShader(ShaderProgram::createShaderProgram("enemiesVs.vs", levelFs)),
        _sceneSpecialsShader(ShaderProgram::createShaderProgram("specialsVs.vs", levelFs)),
        _sceneBallShader(ShaderProgram::createShaderProgram("ballVs.vs", levelFs)),
        _sceneStarShader(ShaderProgram::createShaderProgram("starVs.vs", "starFs.fs"))
{
}

void LevelProcess::render() const {

    _frameBuffer->bindFrameBuffer();

    _sceneBlocksShader->use();
    _blocks->render(_sceneBlocksShader);

    _sceneItemsShader->use();
    _items->render(_sceneItemsShader);

    _sceneEnemiesShader->use();
    _enemies->render(_sceneEnemiesShader);

    _sceneSpecialsShader->use();
    _specials->render(_sceneSpecialsShader);

    _sceneBallShader->use();
    _ball->render(_sceneBallShader);

    _sceneStarShader->use();
    _star->render(_sceneStarShader);
}

void LevelProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _sceneBlocksShader->freeGPUMemory();
    _sceneEnemiesShader->freeGPUMemory();
    _sceneItemsShader->freeGPUMemory();
    _sceneBallShader->freeGPUMemory();
    _sceneStarShader->freeGPUMemory();
}

const std::string LevelProcess::levelFs = "levelFs.fs";

std::shared_ptr<const GLuint> LevelProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}
