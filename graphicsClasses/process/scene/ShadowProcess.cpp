//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "ShadowProcess.h"

#include <utility>


ShadowProcess::ShadowProcess(
    DepthFrameBuffer_uptr frameBuffer,
    std::vector<std::pair<CstShaderProgram_sptr, RenderPass_sptr>> &&shadersRenderPasses,
    bool isFirst
) :
    _frameBuffer(std::move(frameBuffer)),
    _shadersRenderPasses(std::move(shadersRenderPasses)),
    _isFirst(isFirst) {
}

ShadowProcess_sptr ShadowProcess::createInstance(
    const JBTypes::FileContent &fileContent,
    RenderPass_sptr blocks,
    RenderPass_sptr items,
    RenderPass_sptr enemies,
    RenderPass_sptr specials,
    RenderPass_sptr ball,
    bool isFirst
) {

    const auto shadowDefines =
        isFirst
        ? std::vector<std::string>{"SHADOW_PASS"}
        : std::vector<std::string>{"SHADOW_PASS_2"};

    std::vector<std::pair<CstShaderProgram_sptr, RenderPass_sptr> > shadersRenderPasses{};
    std::vector<std::pair<std::string, RenderPass_sptr> > vertexShaderFilesRenderPasses{
        {"blocksVs.vs",   std::move(blocks)},
        {"itemsMapVs.vs", std::move(items)},
        {"enemiesVs.vs",  std::move(enemies)},
        {"specialsVs.vs", std::move(specials)},
        {"ballVs.vs",     std::move(ball)}
    };
    for (auto &vertexShaderFileRenderPass: vertexShaderFilesRenderPasses) {
        const auto &vertexShaderFile = vertexShaderFileRenderPass.first;
        auto &renderPass = vertexShaderFileRenderPass.second;
        shadersRenderPasses.emplace_back(
            ShaderProgram::createInstance(
                fileContent,
                vertexShaderFile,
                "depthFs.fs",
                renderPass->genUniformNames(),
                shadowDefines
            ),
            std::move(renderPass)
        );
    }
    return std::make_shared<ShadowProcess>(
        DepthFrameBuffer::createInstance(
            sizeDepthTexture,
            sizeDepthTexture
        ),
        std::move(shadersRenderPasses),
        isFirst
    );
}

void ShadowProcess::render() const {

    if (_isFirst) {
        glCullFace(GL_FRONT);
        FrameBuffer::enableDepthTest();
        FrameBuffer::setViewportSize(sizeDepthTexture, sizeDepthTexture);
    }
    _frameBuffer->bindFrameBuffer();
    _frameBuffer->clear();

    for (const auto &shaderRenderPass: _shadersRenderPasses) {
        const auto &shader = shaderRenderPass.first;
        const auto &renderPass = shaderRenderPass.second;
        shader->use();
        renderPass->render(shader);
    }
}

void ShadowProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    for(auto& shaderRenderPass: _shadersRenderPasses) {
        auto& shader = shaderRenderPass.first;
        shader->freeGPUMemory();
    }
}

std::shared_ptr<const GLuint> ShadowProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

vecCstShaderProgram_sptr ShadowProcess::getShaderPrograms() const {
    vecCstShaderProgram_sptr shaderPrograms;
    for(auto& shaderRenderPass: _shadersRenderPasses) {
        auto& shader = shaderRenderPass.first;
        shaderPrograms.push_back(shader);
    }
    return shaderPrograms;
}

