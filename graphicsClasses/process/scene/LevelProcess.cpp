//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "LevelProcess.h"

LevelProcess::LevelProcess(
    GLsizei width,
    GLsizei height,
    GLuint shadowTexture,
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
   _shadowTexture(shadowTexture),
   _sceneBlocksShader(createLevelProcessShaderProgram("blocksVs.vs")),
   _sceneItemsShader(createLevelProcessShaderProgram("itemsMapVs.vs")),
   _sceneEnemiesShader(createLevelProcessShaderProgram("enemiesVs.vs")),
   _sceneSpecialsShader(createLevelProcessShaderProgram("specialsVs.vs")),
   _sceneBallShader(createLevelProcessShaderProgram("ballVs.vs")),
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

std::shared_ptr<const GLuint> LevelProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

CstShaderProgram_sptr LevelProcess::createLevelProcessShaderProgram(const std::string &vs) const {
    CstShaderProgram_sptr sp = ShaderProgram::createShaderProgram(vs, levelFs);
    sp->use();
    sp->bindUniformTexture("depthTexture", 0, _shadowTexture);
    return sp;
}

const std::string LevelProcess::levelFs = "levelFs.fs";

