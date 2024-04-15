/*
 * File: SceneRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#include "SceneRendering.h"
#include "process/scene/LevelProcess.h"
#include "process/scene/PostEffects.h"

SceneRendering::SceneRendering(const Scene& scene,
                               GLsizei width,
                               GLsizei height,
                               LevelProcess_uptr levelProcess,
                               PostEffects_uptr postEffects,
                               SceneUniformBuffer&& sceneUniformBuffer)
    : Rendering(width, height),
      _scene(scene),
      _levelProcess(std::move(levelProcess)),
      _postEffects(std::move(postEffects)),
      _sceneUniformBuffer(std::move(sceneUniformBuffer)) {}

std::unique_ptr<SceneRendering> SceneRendering::createInstance(const Scene& scene,
                                                               GLsizei width,
                                                               GLsizei height,
                                                               GLint defaultFrameBuffer,
                                                               const JBTypes::FileContent& fileContent,
                                                               RenderingCache& renderingCache) {
    // Alpha
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Culling
    glEnable(GL_CULL_FACE);

    auto levelProcess = LevelProcess::createInstance(fileContent, width, height, scene.getMap(),
                                                     scene.getStar(), scene.getStar2(),
                                                     scene.getPlayer()->getCurrentBallSkin(), renderingCache);

    constexpr size_t expensivePostProcessHeight = 192;
    const auto expensivePostProcessWidth =
        static_cast<GLsizei>(static_cast<float>(expensivePostProcessHeight) * static_cast<float>(width) /
                             static_cast<float>(height));

    const auto expensivePostProcessWidthGLsizei = static_cast<GLsizei>(expensivePostProcessWidth);
    const auto expensivePostProcessHeightGLsizei = static_cast<GLsizei>(expensivePostProcessHeight);

    auto postEffects = std::unique_ptr<PostEffects>(new PostEffects(
        fileContent, width, height, expensivePostProcessWidthGLsizei, expensivePostProcessHeightGLsizei,
        levelProcess->getRenderTexture(), defaultFrameBuffer, renderingCache));

    auto shadersProgramsUsingUniformBuffer = levelProcess->getShaderPrograms();
    auto postEffectsShaderPrograms = postEffects->getShaderPrograms();
    shadersProgramsUsingUniformBuffer.insert(shadersProgramsUsingUniformBuffer.end(),
                                             postEffectsShaderPrograms.begin(),
                                             postEffectsShaderPrograms.end());

    return std::unique_ptr<SceneRendering>(
        new SceneRendering(scene, width, height, std::move(levelProcess), std::move(postEffects),
                           SceneUniformBuffer(shadersProgramsUsingUniformBuffer)));
}

void SceneRendering::update() {
    const auto& sceneCamera = _scene.getCamera();
    const auto& sceneBall = _scene.getBall();
    _sceneUniformBuffer.update(sceneCamera->viewProjection(), Camera::genVPMatrixFromStar(*_scene.getStar()),
                               Camera::genVPMatrixFromStar((*_scene.getStar2())), sceneCamera->pos(),
                               Utility::convertToOpenGLFormat(_scene.getStar()->lightDirection()),
                               Utility::convertToOpenGLFormat(_scene.getStar2()->lightDirection()),
                               Utility::colorAsVec3(sceneBall->getTeleportationColor()),
                               glm::vec1(sceneBall->getTeleportationCoefficient()));

    _levelProcess->update();
}
vecShaderProgram_sptr SceneRendering::getShaderPrograms() {
    return _levelProcess->getShaderPrograms()
}

void SceneRendering::render() const {
    _levelProcess->render();
    _postEffects->render();
}
