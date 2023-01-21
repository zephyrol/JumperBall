//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#include "LevelProcess.h"

#include <utility>

LevelProcess::LevelProcess(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint shadowTexture,
    GLuint shadow2Texture,
    RenderPass_sptr blocks,
    RenderPass_sptr items,
    RenderPass_sptr enemies,
    RenderPass_sptr specials,
    RenderPass_sptr ball,
    RenderPass_sptr star
) : _width(width),
    _height(height),
    _frameBuffer(ColorableFrameBuffer::createInstance(
        _width,
        _height,
        true,
        true,
        std::unique_ptr<glm::vec3>(new glm::vec3(0.f, 0.f, 0.1f))
    )),
    _blocks(std::move(blocks)),
    _items(std::move(items)),
    _enemies(std::move(enemies)),
    _specials(std::move(specials)),
    _ball(std::move(ball)),
    _star(std::move(star)),
    _shadowTexture(shadowTexture),
    _shadow2Texture(shadow2Texture),
    _sceneBlocksShader(createLevelProcessShaderProgram(
        fileContent,
        "blocksVs.vs",
        _blocks->genUniformNames()
        ) ),
    _sceneItemsShader(createLevelProcessShaderProgram(
        fileContent,
        "itemsMapVs.vs",
        _items->genUniformNames()
        )),
    _sceneEnemiesShader(
        createLevelProcessShaderProgram(
            fileContent,
            "enemiesVs.vs",
            _enemies->genUniformNames()
            )),
    _sceneSpecialsShader(
        createLevelProcessShaderProgram(
            fileContent,
            "specialsVs.vs",
            _specials->genUniformNames()
            )),
    _sceneBallShader(
        createLevelProcessShaderProgram(
            fileContent,
            "ballVs.vs",
            _ball->genUniformNames()
        )),
    _sceneStarShader(ShaderProgram::createInstance(
        fileContent,
        "starVs.vs",
        "starFs.fs",
        _star->genUniformNames())
    ),
    _shadersRenderPasses(
        {
            {_sceneBlocksShader,   _blocks},
            {_sceneItemsShader,    _items},
            {_sceneEnemiesShader,  _enemies},
            {_sceneSpecialsShader, _specials},
            {_sceneBallShader,     _ball},
            {_sceneStarShader,     _star}
        }
    ) {
}

void LevelProcess::render() const {

    glCullFace(GL_BACK);
    FrameBuffer::setViewportSize(_width, _height);
    _frameBuffer->bindFrameBuffer();
    _frameBuffer->clear();

    ShaderProgram::setActiveTexture(1);
    ShaderProgram::bindTexture(_shadow2Texture);

    ShaderProgram::setActiveTexture(0);
    ShaderProgram::bindTexture(_shadowTexture);

    for (const auto &shaderRenderPass: _shadersRenderPasses) {
        const auto &shader = shaderRenderPass.first;
        const auto &renderPass = shaderRenderPass.second;
        shader->use();
        renderPass->render(shader);
    }
}

void LevelProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    _sceneBlocksShader->freeGPUMemory();
    _sceneEnemiesShader->freeGPUMemory();
    _sceneItemsShader->freeGPUMemory();
    _sceneBallShader->freeGPUMemory();
    _sceneSpecialsShader->freeGPUMemory();
    _sceneStarShader->freeGPUMemory();
}

std::shared_ptr<const GLuint> LevelProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

CstShaderProgram_sptr LevelProcess::createLevelProcessShaderProgram(
    const JBTypes::FileContent &fileContent,
    const std::string &vs,
    std::vector<std::string>&& uniformNames
) {
    constexpr auto depthTextureName = "depthTexture";
    constexpr auto depth2TextureName = "depth2Texture";

    uniformNames.emplace_back(depthTextureName);
    uniformNames.emplace_back(depth2TextureName);
    auto shader = ShaderProgram::createInstance(
        fileContent,
        vs,
        "levelFs.fs",
        uniformNames,
        {"LEVEL_PASS"}
    );
    shader->use();
    shader->bindUniformTextureIndex(depthTextureName, 0);
    shader->bindUniformTextureIndex(depth2TextureName, 1);
    return shader;
}

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

