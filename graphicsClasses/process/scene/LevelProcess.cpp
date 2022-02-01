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
):  _width(width),
    _height(height),
    _frameBuffer(
        new FrameBuffer(
                _width,
                _height,
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

    FrameBuffer::setViewportSize(_width, _height);
    _frameBuffer->bindFrameBuffer();
    _frameBuffer->clean();

    for (const auto& shaderRenderPass : std::map<CstShaderProgram_sptr, RenderPass_sptr>({
        { _sceneBlocksShader, _blocks },
        { _sceneItemsShader, _items },
        { _sceneEnemiesShader, _enemies },
        { _sceneSpecialsShader, _specials },
        { _sceneBallShader, _ball }
    })){
        const auto& shader = shaderRenderPass.first;
        const auto& renderPass = shaderRenderPass.second;
        shader->use();
        shader->bindUniformTexture("depthTexture", 0, _shadowTexture);
        renderPass->render(shader);
    }

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

CstShaderProgram_sptr LevelProcess::createLevelProcessShaderProgram(const std::string &vs) {
    return ShaderProgram::createShaderProgram(vs, levelFs, levelDefines);
}

const std::string LevelProcess::levelFs = "levelFs.fs";
const std::vector<std::string> LevelProcess::levelDefines = {"LEVEL_PASS"};

vecCstShaderProgram_sptr LevelProcess::getShaderPrograms() const {
    return {
        _sceneBlocksShader,
        _sceneItemsShader,
        _sceneEnemiesShader,
        _sceneSpecialsShader,
        _sceneBallShader,
        _sceneStarShader
    };
}

