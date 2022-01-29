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
    _renderPasses{ _blocks, _items, _enemies, _specials, _ball, _ball, _screen },
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
    })
    {

    // TODO: Move thoses instructions
    // alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // -----
    glEnable(GL_CULL_FACE);
}

void SceneRendering::update() {
    /*
    const auto updateMat4VariablesFct =
        [this] (const Mesh::UniformVariables_uptr <glm::mat4>& uniformMatrices) {
            uniformMatrices->at(VPStarName) = Camera::genVPMatrixFromStar(*_scene.getStar());
        };

    const auto createMat4Variables =
        [this] ()->Mesh::UniformVariables <glm::mat4> {
            return {
                { SceneRendering::VPStarName, Camera::genVPMatrixFromStar(*_scene.getStar()) },
                { SceneRendering::VPName, _scene.getCamera()->viewProjection() }
            };
        };


    // Block uniform
    renderPass->upsertUniform(
        shaderProgramID,
        VPName,
        Camera::genVPMatrixFromStar(*_scene.getStar())
    );

    renderPass->upsertUniform(
        shaderProgramID,
        SceneRendering::VPName,
        _scene.getCamera()->viewProjection());
    const std::string& VPStarName = SceneRendering::VPStarName;
    renderPass->upsertUniform(shaderProgramID, VPStarName, getUniformMatrix(VPStarName));
    renderPass->upsertUniform(
        shaderProgramID,
        SceneRendering::positionCameraName,
        _scene.getCamera()->pos()
    );

    renderPass->upsertUniform(shaderProgramID, SceneRendering::VPName, camera->viewProjection());
    renderPass->upsertUniform(shaderProgramID, SceneRendering::positionCameraName, camera->pos());

    uniformBlocks->at(lightName)->update(
        Star::lightDirectionName,
        Utility::convertToOpenGLFormat(_scene.getStar()->lightDirection())
    );


    // Todo refactor
    const auto& uniformBlocks = _externalUniformBlocks.uniformBlockVariables;
    const auto& updatingBlocksFct = _externalUniformBlocks.uniformBlockVariablesUpdatingFct;
    updatingBlocksFct(uniformBlocks);

    const auto& uniformMatrices = _externalUniformMatrices.uniformVariables;
    const auto& updatingMatricesFct = _externalUniformMatrices.uniformVariablesUpdatingFct;
    updatingMatricesFct(uniformMatrices);
    */
    for (const auto& renderPass : _renderPasses) {
        renderPass->update();
    }
}

void SceneRendering::render() const {
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

    // TODO: dispose ubo
}


const std::string SceneRendering::VPName = "VP";
const std::string SceneRendering::VPStarName = "VPStar";
const std::string SceneRendering::positionCameraName = "positionCamera";
const std::string SceneRendering::lightName = "light";
