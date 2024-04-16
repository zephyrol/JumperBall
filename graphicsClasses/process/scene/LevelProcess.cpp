//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "LevelProcess.h"
#include "componentsGeneration/MapGroupGenerator.h"
#include "componentsGeneration/StarGroupGenerator.h"

LevelProcess::LevelProcess(GLsizei width,
                           GLsizei height,
                           std::string uniformBufferName,
                           DepthFrameBuffer_uptr firstShadow,
                           DepthFrameBuffer_uptr firstBlankShadow,
                           DepthFrameBuffer_uptr secondShadow,
                           DepthFrameBuffer_uptr secondBlankShadow,
                           ColorableFrameBuffer_uptr levelFrameBuffer,
                           RenderGroup_sptr mapGroup,
                           ShaderProgram_uptr mapShaderProgram,
                           RenderGroup_sptr starGroup,
                           ShaderProgram_uptr starShaderProgram)
    : _width(width),
      _height(height),
      _uniformBufferName(std::move(uniformBufferName)),
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
      _passIdUniformLocation(_mapShaderProgram->getUniformLocation("passId")) {}

LevelProcess_uptr LevelProcess::createInstance(const JBTypes::FileContent& fileContent,
                                               GLsizei width,
                                               GLsizei height,
                                               CstMap_sptr map,
                                               CstStar_sptr firstStar,
                                               CstStar_sptr secondStar,
                                               GLuint uniformBufferBindingPoint,
                                               const std::string& uniformBufferName,
                                               unsigned int ballSkin,
                                               RenderingCache& renderingCache) {
    const MapGroupGenerator mapGroupGenerator(std::move(map), ballSkin);
    auto mapGroup = mapGroupGenerator.genRenderGroup();

    const StarGroupGenerator starGroupGenerator(std::move(firstStar), std::move(secondStar));
    auto starGroup = starGroupGenerator.genRenderGroup();

    const auto starIdsCount = starGroup->numberOfDynamicsIds();
    const std::string starShaderHash = "star;" + std::to_string(starIdsCount);

    auto starShaderProgram = renderingCache.getShaderProgram(starShaderHash);
    if (starShaderProgram == nullptr) {
        starShaderProgram = ShaderProgram::createInstance(
            fileContent, "starVs.vs", "starFs.fs", starShaderHash, {}, {{"idCount", starIdsCount}}, {}, {},
            {{uniformBufferName, uniformBufferBindingPoint}});
    }

    auto mapShaderProgram =
        createMapShaderProgram(fileContent, mapGroup->numberOfDynamicsIds(), uniformBufferBindingPoint,
                               uniformBufferName, renderingCache);

    const auto genDepthTexture = []() {
        return DepthFrameBuffer::createInstance(depthTexturesSize, depthTexturesSize);
    };
    const auto genBlankDepthTexture = []() {
        constexpr GLsizei blankDepthTexturesSize = 1;
        return DepthFrameBuffer::createInstance(blankDepthTexturesSize, blankDepthTexturesSize);
    };

    return std::unique_ptr<LevelProcess>(new LevelProcess(
        width, height, uniformBufferName, genDepthTexture(), genBlankDepthTexture(), genDepthTexture(),
        genBlankDepthTexture(),
        ColorableFrameBuffer::createInstance(width, height, true, true,
                                             std::unique_ptr<glm::vec3>(new glm::vec3(0.f, 0.f, 0.1f))),
        std::move(mapGroup), std::move(mapShaderProgram), std::move(starGroup),
        std::move(starShaderProgram)));
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
    _firstBlankShadow->getRenderTexture()->bind();
    TextureSampler::setActiveTexture(secondShadowTextureIndex);
    _secondBlankShadow->getRenderTexture()->bind();

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

    _secondShadow->getRenderTexture()->bind();
    TextureSampler::setActiveTexture(firstShadowTextureIndex);
    _firstShadow->getRenderTexture()->bind();

    _levelFrameBuffer->bindFrameBuffer();
    _mapShaderProgram->setInteger(_passIdUniformLocation, 3);
    _mapGroup->render();
}

void LevelProcess::update() {
    _mapGroupUniforms.update();
    _starGroupUniforms.update();
}

const CstTextureSampler_uptr& LevelProcess::getRenderTexture() const {
    return _levelFrameBuffer->getRenderTexture();
}

GLsizeiptr LevelProcess::getUniformBufferSize() const {
    return _mapShaderProgram->getUniformBufferSize(_uniformBufferName);
}

std::vector<GLint> LevelProcess::getUniformBufferFieldOffsets(
    const std::vector<std::string>& fieldNames) const {
    return _mapShaderProgram->getUniformBufferFieldOffsets(fieldNames);
}

ShaderProgram_uptr LevelProcess::createMapShaderProgram(const JBTypes::FileContent& fileContent,
                                                        short idCount,
                                                        GLuint uniformBufferBindingPoint,
                                                        const std::string& uniformBufferName,
                                                        RenderingCache& renderingCache) {
    const std::string shaderHash("map;" + std::to_string(idCount));

    auto shader = renderingCache.getShaderProgram(shaderHash);
    if (shader == nullptr) {
        shader = ShaderProgram::createInstance(fileContent, "mapVs.vs", "mapFs.fs", shaderHash, {},
                                               {{"idCount", idCount}}, {}, {},
                                               {{uniformBufferName, uniformBufferBindingPoint}});
    }
    shader->use();
    shader->setTextureIndex("depthTexture", firstShadowTextureIndex);
    shader->setTextureIndex("depth2Texture", secondShadowTextureIndex);

    constexpr auto shadowPixelSize = 1.f / static_cast<float>(depthTexturesSize);

    shader->setUniformArrayVec4(
        "shadowOffsets[0]",
        {shadowPixelSize, shadowPixelSize, 0.0, 0.0, -shadowPixelSize, shadowPixelSize, 0.0, 0.0,
         shadowPixelSize, -shadowPixelSize, 0.0, 0.0, -shadowPixelSize, -shadowPixelSize, 0.0, 0.0});
    return shader;
}
