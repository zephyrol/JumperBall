/*
 * File: SceneRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#include "SceneRendering.h"

SceneRendering::SceneRendering(const Scene& scene, GLsizei width, GLsizei height):
    Rendering(width, height),
    _expensivePreprocessWidth(static_cast<GLsizei>(
        static_cast<float>(SceneRendering::heightBloomTexture)
        * static_cast<float>(width) / static_cast<float>(height)
    )),
    _expensivePreprocessHeight(SceneRendering::heightBloomTexture),
    _scene(scene),
    _blocks(std::make_shared <RenderPass>(MeshGenerator::genBlocks(scene.getMap()))),
    _items(std::make_shared<RenderPass>(MeshGenerator::genItems(scene.getMap()))),
    _enemies(std::make_shared <RenderPass>(MeshGenerator::genEnemies(scene.getMap()))),
    _specials(std::make_shared <RenderPass>(MeshGenerator::genSpecials(scene.getMap()))),
    _ball(std::make_shared <RenderPass>(MeshGenerator::genBall(scene.getBall()))),
    _star(std::make_shared <RenderPass>(MeshGenerator::genStar(scene.getStar()))),
    _screen(std::make_shared <RenderPass>(MeshGenerator::genScreen())),
    _renderPasses{ _blocks, _items, _enemies, _specials, _ball, _star, _screen },
    _shadowStar(std::make_shared<ShadowProcess>(_blocks, _items, _enemies, _specials, _ball )),
    _sceneRenderingProcess(std::make_shared<LevelProcess>(
        _width,
        _height,
        *_shadowStar->getRenderTexture(),
        _blocks,
        _items,
        _enemies,
        _specials,
        _ball,
        _star)),
    _brightPassFilter(std::make_shared<BrightPassFilterProcess>(
        _expensivePreprocessWidth,
        _expensivePreprocessHeight,
        *_sceneRenderingProcess->getRenderTexture(),
        _screen
        )),
    _horizontalBlur(std::make_shared<HorizontalBlurProcess>(
        _expensivePreprocessWidth,
        _expensivePreprocessHeight,
        *_brightPassFilter->getRenderTexture(),
        _screen
        )),
    _verticalBlur(std::make_shared<VerticalBlurProcess>(
        _expensivePreprocessWidth,
        _expensivePreprocessHeight,
        *_horizontalBlur->getRenderTexture(),
        _screen
    )),
    _bloom(std::make_shared<BloomProcess>(
        width,
        height,
        *_sceneRenderingProcess->getRenderTexture(),
        *_verticalBlur->getRenderTexture(),
        _screen
    )),
    _processes({
        _shadowStar,
        _sceneRenderingProcess,
        _brightPassFilter,
        _horizontalBlur,
        _verticalBlur,
        _bloom
    }),
    _sceneUniformBuffer(getShaderProgramsUsingUniformBuffer())
    {

    // TODO: Move thoses instructions
    // alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // -----
    glEnable(GL_CULL_FACE);
}

void SceneRendering::update() {

    const auto& sceneCamera = _scene.getCamera();
    const auto& sceneBall = _scene.getBall();
    _sceneUniformBuffer.update(
        sceneCamera->viewProjection(),
        Camera::genVPMatrixFromStar(*_scene.getStar()),
        sceneCamera->pos(),
        Utility::convertToOpenGLFormat(_scene.getStar()->lightDirection()),
        Utility::colorAsVec3(sceneBall->getTeleportationColor()),
        glm::vec1(sceneBall->getTeleportationCoefficient())
    );

    for (const auto& renderPass : _renderPasses) {
        renderPass->update();
    }
}

void SceneRendering::render() const {

    _sceneUniformBuffer.bindBuffer();
    for (const auto &process: _processes) {
        process->render();
    }
}

void SceneRendering::freeGPUMemory() {

    for (const auto &renderPass: _renderPasses) {
        renderPass->freeGPUMemory();
    }

    for (const auto &process: _processes) {
        process->freeGPUMemory();
    }

    _sceneUniformBuffer.freeGPUMemory();
}


vecCstShaderProgram_sptr SceneRendering::getShaderProgramsUsingUniformBuffer() const {
    auto shadersPrograms = _shadowStar->getShaderPrograms();

    const auto levelShaderPrograms = _sceneRenderingProcess->getShaderPrograms();
    shadersPrograms.insert(shadersPrograms.end(),levelShaderPrograms.begin(), levelShaderPrograms.end());

    const auto bloomShaderProgram = _bloom->getShaderPrograms();
    shadersPrograms.insert(shadersPrograms.end(),bloomShaderProgram.begin(), bloomShaderProgram.end());

    return shadersPrograms;
}
