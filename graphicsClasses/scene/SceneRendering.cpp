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
    vecRenderPass_sptr &&renderPasses,
    vecRenderProcess_sptr &&processes,
    SceneUniformBuffer &&sceneUniformBuffer
) :
    Rendering(width, height),
    _scene(scene),
    _renderPasses(std::move(renderPasses)),
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Culling
    glEnable(GL_CULL_FACE);

    constexpr size_t expensivePreprocessHeight = 192;
    const auto expensivePreprocessWidth = static_cast<GLsizei>(
        static_cast<float>(expensivePreprocessHeight)
        * static_cast<float>(width) / static_cast<float>(height)
    );
    const auto blocks = std::make_shared<RenderPass>(MeshGenerator::genBlocks(scene.getMap()));
    const auto items = std::make_shared<RenderPass>(MeshGenerator::genItems(scene.getMap()));
    const auto enemies = std::make_shared<RenderPass>(MeshGenerator::genEnemies(scene.getMap()));
    const auto specials = std::make_shared<RenderPass>(MeshGenerator::genSpecials(scene.getMap()));
    const auto ball = std::make_shared<RenderPass>(MeshGenerator::genBall(scene.getBall()));
    const auto star = std::make_shared<RenderPass>(
        MeshGenerator::genStars(scene.getStar(), scene.getStar2())
    );
    const auto screen = std::make_shared<RenderPass>(MeshGenerator::genScreen());


    std::vector<std::shared_ptr<ShadowProcess> > shadowProcesses{};
    for (const auto order: {true, false}) {
        shadowProcesses.emplace_back(
            std::make_shared<ShadowProcess>(
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
    const auto sceneRenderingProcess = std::make_shared<LevelProcess>(
        fileContent,
        width,
        height,
        *shadowProcesses.front()->getRenderTexture(),
        *shadowProcesses.back()->getRenderTexture(),
        blocks,
        items,
        enemies,
        specials,
        ball,
        star
    );

    const auto brightPassFilter = std::make_shared<BrightPassFilterProcess>(
        fileContent,
        expensivePreprocessWidth,
        expensivePreprocessHeight,
        *sceneRenderingProcess->getRenderTexture(),
        screen
    );

    const auto horizontalBlur = std::make_shared<HorizontalBlurProcess>(
        fileContent,
        expensivePreprocessWidth,
        expensivePreprocessHeight,
        *brightPassFilter->getRenderTexture(),
        screen
    );
    const auto verticalBlur = std::make_shared<VerticalBlurProcess>(
        fileContent,
        expensivePreprocessWidth,
        expensivePreprocessHeight,
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

    return std::unique_ptr<SceneRendering>(new SceneRendering(
        scene,
        width,
        height,
        std::initializer_list<RenderPass_sptr>({blocks, items, enemies, specials, ball, star, screen}),
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
        glm::vec1(sceneBall->getTeleportationCoefficient())
    );

    for (const auto &renderPass: _renderPasses) {
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
    _sceneUniformBuffer.freeGPUMemory();
}

