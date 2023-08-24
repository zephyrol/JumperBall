/*
 * File: SceneRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#include "SceneRendering.h"
#include "process/scene/ShadowProcess.h"
#include "process/scene/LevelProcess.h"
#include "process/scene/PostEffects.h"
#include "componentsGeneration/MapGroupGenerator.h"
#include "componentsGeneration/StarGroupGenerator.h"
#include "componentsGeneration/ScreenGroupGenerator.h"

SceneRendering::SceneRendering(
    const Scene &scene,
    GLsizei width,
    GLsizei height,
    vecRenderGroup_sptr &&renderGroups,
    vecRenderProcess_sptr &&processes,
    SceneUniformBuffer &&sceneUniformBuffer
) :
    Rendering(width, height),
    _scene(scene),
    _renderGroups(std::move(renderGroups)),
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

    constexpr size_t expensivePostProcessHeight = 192;
    const auto expensivePostProcessWidth = static_cast<GLsizei>(
        static_cast<float>(expensivePostProcessHeight)
        * static_cast<float>(width) / static_cast<float>(height)
    );

    const MapGroupGenerator mapGroupGenerator (scene.getMap(), scene.getPlayer()->getCurrentBallSkin());
    const auto mapGroup = mapGroupGenerator.genRenderGroup();

    const StarGroupGenerator starGroupGenerator(scene.getStar(), scene.getStar2());
    const auto starGroup = starGroupGenerator.genRenderGroup();

    const ScreenGroupGenerator screenGroupGenerator;
    const auto screenGroup = screenGroupGenerator.genRenderGroup();

    /*std::vector<std::shared_ptr<ShadowProcess> > shadowProcesses{};
    for (const auto order: {true, false}) {
        shadowProcesses.emplace_back(
            ShadowProcess::createInstance(
                fileContent,
                ball,
                order
            )
        );
    }*/
    const auto levelProcess = LevelProcess::createInstance(
        fileContent,
        width,
        height,
        0,//*shadowProcesses.front()->getRenderTexture(),
        0,//*shadowProcesses.back()->getRenderTexture(),
        1024,//shadowProcesses.front()->depthTextureSize(),
        mapGroup,
        starGroup
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
        defaultFrameBuffer,
        screenGroup
    );

    //auto shadersProgramsUsingUniformBuffer = shadowProcesses.front()->getShaderPrograms();
    auto shadersProgramsUsingUniformBuffer = levelProcess->getShaderPrograms();

    for (const auto &shaderPrograms: {
        //shadowProcesses.back()->getShaderPrograms(),
        // levelProcess->getShaderPrograms(),
        postEffects->getShaderPrograms()
    }) {
        shadersProgramsUsingUniformBuffer.insert(
            shadersProgramsUsingUniformBuffer.end(),
            shaderPrograms.begin(),
            shaderPrograms.end()
        );
    }

    std::vector<RenderGroup_sptr> renderGroups {mapGroup, starGroup, screenGroup} ;
    return std::unique_ptr<SceneRendering>(new SceneRendering(
        scene,
        width,
        height,
        std::move(renderGroups),
        std::initializer_list<RenderProcess_sptr>(
            {
                // shadowProcesses.front(),
                // shadowProcesses.back(),
                levelProcess,
                postEffects
            }
        ),
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
        glm::vec1(sceneBall->getTeleportationCoefficient())
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

    for (const auto &renderGroup: _renderGroups) {
        renderGroup->freeGPUMemory();
    }
    for (const auto &process: _processes) {
        process->freeGPUMemory();
    }
    _sceneUniformBuffer.freeGPUMemory();
}

