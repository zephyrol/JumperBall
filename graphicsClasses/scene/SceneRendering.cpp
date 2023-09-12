/*
 * File: SceneRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#include "SceneRendering.h"
#include "process/scene/LevelProcess.h"
#include "process/scene/PostEffects.h"

SceneRendering::SceneRendering(
    const Scene &scene,
    GLsizei width,
    GLsizei height,
    vecRenderProcess_sptr &&processes,
    SceneUniformBuffer &&sceneUniformBuffer
) :
    Rendering(width, height),
    _scene(scene),
    _processes(std::move(processes)),
    _sceneUniformBuffer(std::move(sceneUniformBuffer)) {
}

std::unique_ptr<SceneRendering> SceneRendering::createInstance(
    const Scene &scene,
    GLsizei width,
    GLsizei height,
    GLint defaultFrameBuffer,
    const JBTypes::FileContent &fileContent
) {

    // Alpha
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Culling
    glEnable(GL_CULL_FACE);

    const auto levelProcess = LevelProcess::createInstance(
        fileContent,
        width,
        height,
        scene.getMap(),
        scene.getStar(),
        scene.getStar2(),
        scene.getPlayer()->getCurrentBallSkin()
    );

    constexpr size_t expensivePostProcessHeight = 192;
    const auto expensivePostProcessWidth = static_cast<GLsizei>(
        static_cast<float>(expensivePostProcessHeight)
        * static_cast<float>(width) / static_cast<float>(height)
    );

    const auto expensivePostProcessWidthGLsizei = static_cast<GLsizei>(expensivePostProcessWidth);
    const auto expensivePostProcessHeightGLsizei = static_cast<GLsizei>(expensivePostProcessHeight);

    const auto postEffects = std::make_shared<PostEffects>(
        fileContent,
        width,
        height,
        expensivePostProcessWidthGLsizei,
        expensivePostProcessHeightGLsizei,
        *levelProcess->getRenderTexture(),
        defaultFrameBuffer
    );

    auto shadersProgramsUsingUniformBuffer = levelProcess->getShaderPrograms();
    auto postEffectsShaderPrograms = postEffects->getShaderPrograms();
    shadersProgramsUsingUniformBuffer.insert(
        shadersProgramsUsingUniformBuffer.end(),
        postEffectsShaderPrograms.begin(),
        postEffectsShaderPrograms.end()
    );

    return std::unique_ptr<SceneRendering>(new SceneRendering(
        scene,
        width,
        height,
        std::initializer_list<RenderProcess_sptr>({levelProcess, postEffects}),
        SceneUniformBuffer(shadersProgramsUsingUniformBuffer)
    ));
}

void SceneRendering::update() {

    const auto &sceneCamera = _scene.getCamera();
    const auto &sceneBall = _scene.getBall();
    _sceneUniformBuffer.update(
        sceneCamera->viewProjection(),
        Camera::genVPMatrixFromStar(*_scene.getStar()),
        Camera::genVPMatrixFromStar((*_scene.getStar2())),
        sceneCamera->pos(),
        Utility::convertToOpenGLFormat(_scene.getStar()->lightDirection()),
        Utility::convertToOpenGLFormat(_scene.getStar2()->lightDirection()),
        Utility::colorAsVec3(sceneBall->getTeleportationColor()),
        glm::vec1(sceneBall->getTeleportationCoefficient()),
        Utility::convertToOpenGLFormat(sceneBall->get3DPosition()),
        sceneCamera->ballLookingDirection()
    );

    for (const auto &process: _processes) {
        process->update();
    }
}

void SceneRendering::render() const {
    for (const auto &process: _processes) {
        process->render();
    }
}

void SceneRendering::freeGPUMemory() {
    for (const auto &process: _processes) {
        process->freeGPUMemory();
    }
    _sceneUniformBuffer.freeGPUMemory();
}

