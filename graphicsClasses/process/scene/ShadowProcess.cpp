//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "ShadowProcess.h"

#include <utility>


ShadowProcess::ShadowProcess(
    DepthFrameBuffer_uptr frameBuffer,
    std::vector<std::pair<ShaderProgram_sptr, RenderPass_sptr>> &&shadersRenderPasses,
    bool isFirst,
    GLsizei depthTextureSize
) :
    _frameBuffer(std::move(frameBuffer)),
    _shadersRenderPasses(std::move(shadersRenderPasses)),
    _isFirst(isFirst),
    _depthTextureSize(depthTextureSize) {
}

ShadowProcess_sptr ShadowProcess::createInstance(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    CstRenderGroupsManager_sptr blocks,
    CstRenderGroupsManager_sptr items,
    CstRenderGroupsManager_sptr enemies,
    CstRenderGroupsManager_sptr specials,
    CstRenderGroupsManager_sptr ball,
    bool isFirst
) {

    const auto shadowDefines =
        isFirst
        ? std::vector<std::string>{"SHADOW_PASS"}
        : std::vector<std::string>{"SHADOW_PASS_2"};

    std::vector<std::pair<ShaderProgram_sptr, RenderPass_sptr> > shadersRenderPasses{};
    std::vector<std::pair<std::string, CstRenderGroupsManager_sptr> > vertexShaderFilesGroupsManagers{
        {"blocksVs.vs",   std::move(blocks)},
        {"itemsMapVs.vs", std::move(items)},
        {"enemiesVs.vs",  std::move(enemies)},
        {"specialsVs.vs", std::move(specials)},
        {"ballVs.vs",     std::move(ball)}
    };
    for (auto &vertexShaderFileGroupsManager: vertexShaderFilesGroupsManagers) {
        const auto &vertexShaderFile = vertexShaderFileGroupsManager.first;
        const auto &groupsManager = vertexShaderFileGroupsManager.second;

        const auto shaderProgram = ShaderProgram::createInstance(
            fileContent,
            vertexShaderFile,
            "depthFs.fs",
            shadowDefines
        );

        shadersRenderPasses.emplace_back(
            shaderProgram,
            std::make_shared<RenderPass>(shaderProgram, groupsManager)
        );
    }

    const auto nearestPowerOfTwo = std::min(
        2048,
        static_cast<GLsizei>(roundf(powf(
            2.f,
            floorf(logf(std::max(width, height)) / logf(2.f))
        )))
    );

    return std::make_shared<ShadowProcess>(
        DepthFrameBuffer::createInstance(
            nearestPowerOfTwo,
            nearestPowerOfTwo
        ),
        std::move(shadersRenderPasses),
        isFirst,
        nearestPowerOfTwo
    );
}

void ShadowProcess::render() const {

    if (_isFirst) {
        glCullFace(GL_FRONT);
        FrameBuffer::enableDepthTest();
        FrameBuffer::setViewportSize(_depthTextureSize, _depthTextureSize);
    }
    _frameBuffer->bindFrameBuffer();
    _frameBuffer->clear();

    for (const auto &shaderRenderPass: _shadersRenderPasses) {
        const auto &shader = shaderRenderPass.first;
        const auto &renderPass = shaderRenderPass.second;
        shader->use();
        renderPass->render();
    }
}

void ShadowProcess::update() {
    for (const auto &shaderRenderPass: _shadersRenderPasses) {
        const auto &renderPass = shaderRenderPass.second;
        renderPass->update();
    }
}

void ShadowProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    for (auto &shaderRenderPass: _shadersRenderPasses) {
        auto &shader = shaderRenderPass.first;
        shader->freeGPUMemory();
    }
}

std::shared_ptr<const GLuint> ShadowProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

vecCstShaderProgram_sptr ShadowProcess::getShaderPrograms() const {
    vecCstShaderProgram_sptr shaderPrograms;
    for (auto &shaderRenderPass: _shadersRenderPasses) {
        auto &shader = shaderRenderPass.first;
        shaderPrograms.push_back(shader);
    }
    return shaderPrograms;
}

GLsizei ShadowProcess::depthTextureSize() const {
    return _depthTextureSize;
}

