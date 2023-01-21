//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "LevelProcess.h"

#include <utility>


LevelProcess::LevelProcess(
    GLsizei width,
    GLsizei height,
    ColorableFrameBuffer_uptr frameBuffer,
    GLuint shadowTexture,
    GLuint shadow2Texture,
    std::vector<std::pair<CstShaderProgram_sptr, RenderPass_sptr>> &&shadersRenderPasses
) :
    _width(width),
    _height(height),
    _frameBuffer(std::move(frameBuffer)),
    _shadowTexture(shadowTexture),
    _shadow2Texture(shadow2Texture),
    _shadersRenderPasses(std::move(shadersRenderPasses)) {
}

LevelProcess_sptr LevelProcess::createInstance(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint shadowTexture,
    GLuint shadow2Texture,
    RenderPass_sptr blocks,
    RenderPass_sptr items,
    RenderPass_sptr enemies,
    RenderPass_sptr specials,
    RenderPass_sptr ball,
    RenderPass_sptr star
) {

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
            createLevelProcessShaderProgram(fileContent, vertexShaderFile, renderPass->genUniformNames()),
            std::move(renderPass)
        );
    }
    shadersRenderPasses.emplace_back(
        ShaderProgram::createInstance(fileContent, "starVs.vs", "starFs.fs", star->genUniformNames()),
        std::move(star)
    );

    return std::make_shared<LevelProcess>(
        width,
        height,
        ColorableFrameBuffer::createInstance(
            width,
            height,
            true,
            true,
            std::unique_ptr<glm::vec3>(new glm::vec3(0.f, 0.f, 0.1f))
        ),
        shadowTexture,
        shadow2Texture,
        std::move(shadersRenderPasses)
    );
}

void LevelProcess::render() const {

    glCullFace(GL_BACK);
    FrameBuffer::setViewportSize(_width, _height);
    _frameBuffer->bindFrameBuffer();
    _frameBuffer->clear();

    ShaderProgram::setActiveTexture(1);
    ShaderProgram::bindTexture(_shadow2Texture);

    ShaderProgram::setActiveTexture(0);
    ShaderProgram::bindTexture(_shadowTexture);

    for (const auto &shaderRenderPass: _shadersRenderPasses) {
        const auto &shader = shaderRenderPass.first;
        const auto &renderPass = shaderRenderPass.second;
        shader->use();
        renderPass->render(shader);
    }
}

void LevelProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    for(auto& shaderRenderPass: _shadersRenderPasses) {
        auto& shader = shaderRenderPass.first;
        shader->freeGPUMemory();
    }
}

std::shared_ptr<const GLuint> LevelProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

CstShaderProgram_sptr LevelProcess::createLevelProcessShaderProgram(
    const JBTypes::FileContent &fileContent,
    const std::string &vs,
    std::vector<std::string> &&uniformNames
) {
    constexpr auto depthTextureName = "depthTexture";
    constexpr auto depth2TextureName = "depth2Texture";

    uniformNames.emplace_back(depthTextureName);
    uniformNames.emplace_back(depth2TextureName);
    auto shader = ShaderProgram::createInstance(
        fileContent,
        vs,
        "levelFs.fs",
        uniformNames,
        {"LEVEL_PASS"}
    );
    shader->use();
    shader->bindUniformTextureIndex(depthTextureName, 0);
    shader->bindUniformTextureIndex(depth2TextureName, 1);
    return shader;
}

vecCstShaderProgram_sptr LevelProcess::getShaderPrograms() const {
    vecCstShaderProgram_sptr shaderPrograms;
    for(auto& shaderRenderPass: _shadersRenderPasses) {
        auto& shader = shaderRenderPass.first;
        shaderPrograms.push_back(shader);
    }
    return shaderPrograms;
}

