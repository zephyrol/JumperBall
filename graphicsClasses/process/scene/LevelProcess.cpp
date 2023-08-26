//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "LevelProcess.h"
#include "componentsGeneration/MapGroupGenerator.h"
#include "componentsGeneration/StarGroupGenerator.h"

LevelProcess::LevelProcess(
    GLsizei width,
    GLsizei height,
    DepthFrameBuffer_uptr firstShadow,
    DepthFrameBuffer_uptr firstBlankShadow,
    DepthFrameBuffer_uptr secondShadow,
    DepthFrameBuffer_uptr secondBlankShadow,
    ColorableFrameBuffer_uptr levelFrameBuffer,
    RenderGroup_sptr mapGroup,
    ShaderProgram_sptr mapShaderProgram,
    RenderGroup_sptr starGroup,
    ShaderProgram_sptr starShaderProgram
) :
    _width(width),
    _height(height),
    _firstShadow(std::move(firstShadow)),
    _firstBlankShadow(std::move(firstBlankShadow)),
    _secondShadow(std::move(secondShadow)),
    _secondBlankShadow(std::move(secondBlankShadow)),
    _levelFrameBuffer(std::move(levelFrameBuffer)),
    _mapGroup(std::move(mapGroup)),
    _mapShaderProgram(std::move(mapShaderProgram)),
    _mapGroupUniforms(_mapGroup->genUniforms(_mapShaderProgram)),
    _starGroup(std::move(starGroup)),
    _starShaderProgram(std::move(starShaderProgram)),
    _starGroupUniforms(_starGroup->genUniforms(_starShaderProgram)),
    _passIdUniformLocation(_mapShaderProgram->getUniformLocation("passId")) {
}


LevelProcess_sptr LevelProcess::createInstance(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    CstMap_sptr map,
    CstStar_sptr firstStar,
    CstStar_sptr secondStar,
    unsigned int ballSkin
) {

    const MapGroupGenerator mapGroupGenerator(map, ballSkin);
    auto mapGroup = mapGroupGenerator.genRenderGroup();

    const StarGroupGenerator starGroupGenerator(firstStar, secondStar);
    auto starGroup = starGroupGenerator.genRenderGroup();

    auto starShaderProgram = ShaderProgram::createInstance(
        fileContent,
        "starVs.vs",
        "starFs.fs",
        {},
        {{"idCount", starGroup->numberOfDynamicsIds()}}
    );

    auto mapShaderProgram = createMapShaderProgram(
        fileContent,
        mapGroup->numberOfDynamicsIds()
    );

    const auto genDepthTexture = []() {
        return DepthFrameBuffer::createInstance(
            depthTexturesSize,
            depthTexturesSize
        );
    };
    const auto genBlankDepthTexture = []() {
        constexpr GLsizei blankDepthTexturesSize = 1;
        return DepthFrameBuffer::createInstance(
            blankDepthTexturesSize,
            blankDepthTexturesSize
        );
    };

    return std::make_shared<LevelProcess>(
        width,
        height,
        genDepthTexture(),
        genBlankDepthTexture(),
        genDepthTexture(),
        genBlankDepthTexture(),
        ColorableFrameBuffer::createInstance(
            width,
            height,
            true,
            true,
            std::unique_ptr<glm::vec3>(new glm::vec3(0.f, 0.f, 0.1f))
        ),
        std::move(mapGroup),
        std::move(mapShaderProgram),
        std::move(starGroup),
        std::move(starShaderProgram)
    );
}

void LevelProcess::render() const {

    FrameBuffer::disableBlending();
    _levelFrameBuffer->bindFrameBuffer();
    _levelFrameBuffer->clear();

    _starShaderProgram->use();
    _starGroupUniforms.bind();
    _starGroup->bind();
    _starGroup->render();

    glCullFace(GL_FRONT);
    FrameBuffer::enableDepthTest();
    FrameBuffer::setViewportSize(depthTexturesSize, depthTexturesSize);
    _mapShaderProgram->use();

    TextureSampler::setActiveTexture(firstShadowTextureIndex);
    TextureSampler::bind(_firstBlankShadow->getRenderTexture());
    TextureSampler::setActiveTexture(secondShadowTextureIndex);
    TextureSampler::bind(_secondBlankShadow->getRenderTexture());

    _mapGroupUniforms.bind();
    _mapGroup->bind();

    // 1. First shadow
    _firstShadow->bindFrameBuffer();
    _firstShadow->clear();
    _mapShaderProgram->setInteger(_passIdUniformLocation, 0);
    _mapGroup->render();

    // 2. Second shadow
    _secondShadow->bindFrameBuffer();
    _secondShadow->clear();
    _mapShaderProgram->setInteger(_passIdUniformLocation, 1);
    _mapGroup->render();

    // 3. Map
    glCullFace(GL_BACK);

    FrameBuffer::setViewportSize(_width, _height);
    _levelFrameBuffer->bindFrameBuffer();

    TextureSampler::bind(_secondShadow->getRenderTexture());
    TextureSampler::setActiveTexture(firstShadowTextureIndex);
    TextureSampler::bind(_firstShadow->getRenderTexture());

    _levelFrameBuffer->bindFrameBuffer();
    _mapShaderProgram->setInteger(_passIdUniformLocation, 3);
    _mapGroup->render();

}

void LevelProcess::update() {
    _mapGroupUniforms.update();
    _starGroupUniforms.update();
}

void LevelProcess::freeGPUMemory() {
    _firstShadow->freeGPUMemory();
    _firstBlankShadow->freeGPUMemory();
    _secondShadow->freeGPUMemory();
    _secondBlankShadow->freeGPUMemory();
    _levelFrameBuffer->freeGPUMemory();
    _mapShaderProgram->freeGPUMemory();
    _mapGroup->freeGPUMemory();
    _starShaderProgram->freeGPUMemory();
    _starGroup->freeGPUMemory();
}

std::shared_ptr<const GLuint> LevelProcess::getRenderTexture() const {
    return std::make_shared<const GLuint>(_levelFrameBuffer->getRenderTexture());
}

ShaderProgram_sptr LevelProcess::createMapShaderProgram(
    const JBTypes::FileContent &fileContent,
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
    shader->setTextureIndex("depthTexture", firstShadowTextureIndex);
    shader->setTextureIndex("depth2Texture", secondShadowTextureIndex);

    const auto shadowPixelSize = 1.f / static_cast<float>(depthTexturesSize);

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
    return {_mapShaderProgram, _starShaderProgram};
}
