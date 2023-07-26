/*
 * File: SceneRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#include "SceneRendering.h"
#include "process/scene/ShadowProcess.h"
#include "process/scene/LevelProcess.h"
#include "process/scene/BrightPassFilterProcess.h"
#include "process/scene/HorizontalBlurProcess.h"
#include "process/scene/VerticalBlurProcess.h"
#include "process/scene/BloomProcess.h"

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

    constexpr size_t expensivePreprocessHeight = 192;
    const auto expensivePreprocessWidth = static_cast<GLsizei>(
        static_cast<float>(expensivePreprocessHeight)
        * static_cast<float>(width) / static_cast<float>(height)
    );

    auto blocksGroup = MeshGenerator::genBlocks(scene.getMap());
    const auto blocks = blocksGroup ? RenderGroup::createInstance(std::move(blocksGroup)) : nullptr;

    auto itemsGroup = MeshGenerator::genItems(scene.getMap());
    const auto items = itemsGroup ? RenderGroup::createInstance(std::move(itemsGroup)) : nullptr;

    auto enemiesGroup = MeshGenerator::genEnemies(scene.getMap());
    const auto enemies = enemiesGroup ? RenderGroup::createInstance(std::move(enemiesGroup)): nullptr;

    auto specialsGroup = MeshGenerator::genSpecials(scene.getMap());
    const auto specials = specialsGroup ? RenderGroup::createInstance(std::move(specialsGroup)): nullptr;

    const auto ball = RenderGroup::createInstance(MeshGenerator::genBall(
        scene.getBall(), scene.getPlayer()->getCurrentBallSkin())
    );
    const auto star = RenderGroup::createInstance(
        MeshGenerator::genStars(scene.getStar(), scene.getStar2())
    );
    const auto screen = RenderGroup::createInstance(MeshGenerator::genScreen());


    std::vector<std::shared_ptr<ShadowProcess> > shadowProcesses{};
    for (const auto order: {true, false}) {
        shadowProcesses.emplace_back(
            ShadowProcess::createInstance(
                fileContent,
                blocks,
                items,
                enemies,
                specials,
                ball,
                order
            )
        );
    }
    const auto sceneRenderingProcess = LevelProcess::createInstance(
        fileContent,
        width,
        height,
        *shadowProcesses.front()->getRenderTexture(),
        *shadowProcesses.back()->getRenderTexture(),
        shadowProcesses.front()->depthTextureSize(),
        blocks,
        items,
        enemies,
        specials,
        ball,
        star
    );

    const auto expensivePreprocessWidthGLsizei = static_cast<GLsizei>(expensivePreprocessWidth);
    const auto expensivePreprocessHeightGLsizei = static_cast<GLsizei>(expensivePreprocessHeight);

    const auto brightPassFilter = std::make_shared<BrightPassFilterProcess>(
        fileContent,
        expensivePreprocessWidthGLsizei,
        expensivePreprocessHeightGLsizei,
        *sceneRenderingProcess->getRenderTexture(),
        screen
    );

    const auto horizontalBlur = std::make_shared<HorizontalBlurProcess>(
        fileContent,
        expensivePreprocessWidthGLsizei,
        expensivePreprocessHeightGLsizei,
        *brightPassFilter->getRenderTexture(),
        screen
    );
    const auto verticalBlur = std::make_shared<VerticalBlurProcess>(
        fileContent,
        expensivePreprocessWidthGLsizei,
        expensivePreprocessHeightGLsizei,
        *horizontalBlur->getRenderTexture(),
        screen
    );

    const auto bloom = std::make_shared<BloomProcess>(
        fileContent,
        width,
        height,
        *verticalBlur->getRenderTexture(),
        defaultFrameBuffer,
        screen
    );

    auto shadersProgramsUsingUniformBuffer = shadowProcesses.front()->getShaderPrograms();

    for (const auto &shaderPrograms: {
        shadowProcesses.back()->getShaderPrograms(),
        sceneRenderingProcess->getShaderPrograms(),
        bloom->getShaderPrograms()
    }) {
        shadersProgramsUsingUniformBuffer.insert(
            shadersProgramsUsingUniformBuffer.end(),
            shaderPrograms.begin(),
            shaderPrograms.end()
        );
    }

    std::vector<RenderGroup_sptr> renderGroups {blocks, ball, star, screen} ;
    if(items != nullptr) {
        renderGroups.emplace_back(items);
    }

    if(enemies != nullptr) {
        renderGroups.emplace_back(enemies);
    }

    if(specials != nullptr) {
        renderGroups.emplace_back(specials);
    }

    return std::unique_ptr<SceneRendering>(new SceneRendering(
        scene,
        width,
        height,
        std::move(renderGroups),
        std::initializer_list<RenderProcess_sptr>(
            {
                shadowProcesses.front(),
                shadowProcesses.back(),
                sceneRenderingProcess,
                brightPassFilter,
                horizontalBlur,
                verticalBlur,
                bloom
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
        glm::vec1(sceneBall->getTeleportationCoefficient()),
        glm::vec1(sceneBall->burnCoefficient())
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

