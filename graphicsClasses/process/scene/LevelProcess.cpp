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
    std::vector<std::pair<ShaderProgram_sptr, RenderPass_sptr>> &&shadersRenderPasses
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
    GLsizei shadowsResolution,
    CstRenderGroup_sptr map,
    CstRenderGroup_sptr star
) {

    std::vector<std::pair<ShaderProgram_sptr, RenderPass_sptr> > shadersRenderPasses{};

    const auto starShaderProgram = ShaderProgram::createInstance(
        fileContent,
        "starVs.vs",
        "starFs.fs",
        {},
        {{"idCount", star->numberOfDynamicsIds()}}
    );
    shadersRenderPasses.emplace_back(
        starShaderProgram,
        std::make_shared<RenderPass>(starShaderProgram, std::move(star))
    );

    const auto mapShaderProgram = createLevelProcessShaderProgram(
        fileContent,
        shadowsResolution,
        map->numberOfDynamicsIds()
    );

    shadersRenderPasses.emplace_back(
        mapShaderProgram,
        std::make_shared<RenderPass>(mapShaderProgram, std::move(map))
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

    TextureSampler::setActiveTexture(1);
    TextureSampler::bind(_shadow2Texture);

    TextureSampler::setActiveTexture(0);
    TextureSampler::bind(_shadowTexture);

    // Render star
    FrameBuffer::disableDepthTest();
    const auto &starShaderRenderPass = _shadersRenderPasses.front();
    const auto &starShader = starShaderRenderPass.first;
    const auto &starRenderPass = starShaderRenderPass.second;
    starShader->use();
    starRenderPass->render();

    // Render others
    FrameBuffer::enableDepthTest();
    for (auto it = _shadersRenderPasses.begin() + 1; it != _shadersRenderPasses.end(); ++it) {
        const auto &shader = it->first;
        const auto &renderPass = it->second;
        shader->use();
        renderPass->render();
    }

}

void LevelProcess::update() {
    for (const auto &shaderRenderPass: _shadersRenderPasses) {
        const auto &renderPass = shaderRenderPass.second;
        renderPass->update();
    }
}


void LevelProcess::freeGPUMemory() {
    _frameBuffer->freeGPUMemory();
    for (auto &shaderRenderPass: _shadersRenderPasses) {
        auto &shader = shaderRenderPass.first;
        shader->freeGPUMemory();
    }
}

std::shared_ptr<const GLuint> LevelProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_frameBuffer->getRenderTexture());
}

ShaderProgram_sptr LevelProcess::createLevelProcessShaderProgram(
    const JBTypes::FileContent &fileContent,
    GLsizei shadowsResolution,
    short idCount
) {
    auto shader = ShaderProgram::createInstance(
        fileContent,
        "mapVs.vs",
        "mapFs.fs",
        {},
        {{"idCount", idCount}}
    );
    shader->use();
    shader->setTextureIndex("depthTexture", 0);
    shader->setTextureIndex("depth2Texture", 1);

    const auto shadowPixelSize = 1.f / static_cast<float>(shadowsResolution);

    shader->setUniformArrayVec4(
        "shadowOffsets[0]",
        {
            shadowPixelSize, shadowPixelSize, 0.0, 0.0,
            -shadowPixelSize, shadowPixelSize, 0.0, 0.0,
            shadowPixelSize, -shadowPixelSize, 0.0, 0.0,
            -shadowPixelSize, -shadowPixelSize, 0.0, 0.0
        }
    );
    return shader;
}

vecCstShaderProgram_sptr LevelProcess::getShaderPrograms() const {
    vecCstShaderProgram_sptr shaderPrograms;
    for (auto &shaderRenderPass: _shadersRenderPasses) {
        auto &shader = shaderRenderPass.first;
        shaderPrograms.push_back(shader);
    }
    return shaderPrograms;
}

