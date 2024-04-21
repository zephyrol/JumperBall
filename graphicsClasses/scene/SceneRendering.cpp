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
                               UniformBuffer_uptr sceneUniformBuffer)
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

    const std::string uniformBufferName = "Scene";
    constexpr GLuint uniformBufferBindingPoint = 0;
    auto levelProcess =
        LevelProcess::createInstance(fileContent, width, height, scene.getMap(), scene.getStar(),
                                     scene.getStar2(), uniformBufferBindingPoint, uniformBufferName,
                                     scene.getPlayer()->getCurrentBallSkin(), renderingCache);

    constexpr size_t expensivePostProcessHeight = 192;
    const auto expensivePostProcessWidth =
        static_cast<GLsizei>(static_cast<float>(expensivePostProcessHeight) * static_cast<float>(width) /
                             static_cast<float>(height));

    const auto expensivePostProcessWidthGLsizei = static_cast<GLsizei>(expensivePostProcessWidth);
    const auto expensivePostProcessHeightGLsizei = static_cast<GLsizei>(expensivePostProcessHeight);

    auto postEffects = PostEffects::createInstance(fileContent, width, height, expensivePostProcessWidthGLsizei,
                        expensivePostProcessHeightGLsizei, uniformBufferBindingPoint, uniformBufferName,
                        levelProcess->getRenderTexture(), defaultFrameBuffer, renderingCache);
    auto uniformBuffer =
        UniformBuffer::createInstance(uniformBufferBindingPoint, levelProcess->getUniformBufferSize(),
                                      levelProcess->getUniformBufferFieldOffsets(
                                          {"VP", "VPStar", "VPStar2", "cameraPosition", "lightDirection",
                                           "light2Direction", "flashColor", "teleportationCoeff"}));
    return std::unique_ptr<SceneRendering>(new SceneRendering(
        scene, width, height, std::move(levelProcess), std::move(postEffects), std::move(uniformBuffer)));
}

void SceneRendering::update() {
    const auto& sceneCamera = _scene.getCamera();
    const auto& sceneBall = _scene.getBall();

    _sceneUniformBuffer->updateField(0, sceneCamera->viewProjection());
    _sceneUniformBuffer->updateField(1, Camera::genVPMatrixFromStar(*_scene.getStar()));
    _sceneUniformBuffer->updateField(2, Camera::genVPMatrixFromStar((*_scene.getStar2())));
    _sceneUniformBuffer->updateField(3, sceneCamera->pos());
    _sceneUniformBuffer->updateField(4, Utility::convertToOpenGLFormat(_scene.getStar()->lightDirection()));
    _sceneUniformBuffer->updateField(5, Utility::convertToOpenGLFormat(_scene.getStar2()->lightDirection()));
    _sceneUniformBuffer->updateField(6, Utility::colorAsVec3(sceneBall->getTeleportationColor()));
    _sceneUniformBuffer->updateField(7, glm::vec1(sceneBall->getTeleportationCoefficient()));
    _sceneUniformBuffer->updateBufferOnGPU();

    _levelProcess->update();
}

void SceneRendering::render() const {
    _levelProcess->render();
    _postEffects->render();
}

void SceneRendering::fillCache(RenderingCache& renderingCache) {
    _levelProcess->fillCache(renderingCache);
    _postEffects->fillCache(renderingCache);
}
