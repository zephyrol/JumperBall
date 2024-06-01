//
// Created by S.Morgenthaler on 26/01/2022.
//

#include "LevelProcess.h"

#include <random>

#include "componentsGeneration/MapGroupGenerator.h"
#include "componentsGeneration/StarGroupGenerator.h"

LevelProcess::LevelProcess(GLsizei width,
                           GLsizei height,
                           DepthFrameBuffer_uptr firstShadow,
                           DepthFrameBuffer_uptr secondShadow,
                           ColorableFrameBuffer_uptr levelFrameBuffer,
                           CstTextureSampler_uptr shadowKernel,
                           RenderGroup_sptr mapGroup,
                           ShaderProgram_sptr mapShaderProgram,
                           RenderGroup_sptr starGroup,
                           ShaderProgram_sptr starShaderProgram)
    : _width(width),
      _height(height),
      _firstShadow(std::move(firstShadow)),
      _secondShadow(std::move(secondShadow)),
      _levelFrameBuffer(std::move(levelFrameBuffer)),
      _depthKernel(std::move(shadowKernel)),
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
                                               unsigned int ballSkin) {
    const MapGroupGenerator mapGroupGenerator(std::move(map), ballSkin);
    auto mapGroup = mapGroupGenerator.genRenderGroup();

    const StarGroupGenerator starGroupGenerator(std::move(firstStar), std::move(secondStar));
    auto starGroup = starGroupGenerator.genRenderGroup();

    auto starShaderProgram = ShaderProgram::createInstance(fileContent, "starVs.vs", "starFs.fs", {},
                                                           {{"idCount", starGroup->numberOfDynamicsIds()}});

    auto mapShaderProgram =
        createMapShaderProgram(fileContent, mapGroup->numberOfDynamicsIds(), width, height);

    const auto genDepthTexture = []() {
        return DepthFrameBuffer::createInstance(depthTexturesSize, depthTexturesSize);
    };

    return std::unique_ptr<LevelProcess>(new LevelProcess(
        width, height, genDepthTexture(), genDepthTexture(),
        ColorableFrameBuffer::createInstance(width, height, true, true,
                                             std::unique_ptr<glm::vec3>(new glm::vec3(0.f, 0.f, 0.1f))),
        createDepthKernel(), std::move(mapGroup), std::move(mapShaderProgram), std::move(starGroup),
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
    TextureSampler::bindNoTexture();
    TextureSampler::setActiveTexture(secondShadowTextureIndex);
    TextureSampler::bindNoTexture();

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

    TextureSampler::setActiveTexture(kernelTextureIndex);
    _depthKernel->bind();

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

ShaderProgram_sptr LevelProcess::createMapShaderProgram(const JBTypes::FileContent& fileContent,
                                                        short idCount,
                                                        GLsizei width,
                                                        GLsizei height) {
    constexpr auto penumbraHalfSize = 0.1f;
    constexpr auto shadowPixelsSize = 1.f / static_cast<float>(depthTexturesSize);
    auto shader = ShaderProgram::createInstance(
        fileContent, "mapVs.vs", "mapFs.fs", {}, {{"idCount", idCount}},
        {{"penumbraHalfSize", penumbraHalfSize}, {"shadowPixelsSize", shadowPixelsSize}},
        {{"fragCoordToKernelUv",
          {static_cast<float>(width) / static_cast<float>(kernelTextureSize),
           static_cast<float>(height) / static_cast<float>(kernelTextureSize)}}});
    shader->use();
    shader->setTextureIndex("depthTexture", firstShadowTextureIndex);
    shader->setTextureIndex("depth2Texture", secondShadowTextureIndex);
    shader->setTextureIndex("depthKernelTexture", kernelTextureIndex);

    shader->setUniformArrayVec4(
        "shadowOffsets[0]",
        {shadowPixelsSize, shadowPixelsSize, 0.0, 0.0, -shadowPixelsSize, shadowPixelsSize, 0.0, 0.0,
         shadowPixelsSize, -shadowPixelsSize, 0.0, 0.0, -shadowPixelsSize, -shadowPixelsSize, 0.0, 0.0});
    return shader;
}

CstTextureSampler_uptr LevelProcess::createDepthKernel() {
    constexpr GLsizei numberOfChannels = 4;

    std::vector<unsigned char> kernelData{};
    srand((unsigned)time(NULL));

    for (size_t i = 0; i < kernelTextureSize * kernelTextureSize; ++i) {
        const float angle = (float(rand()) / float((RAND_MAX))) * M_PI;
        const auto r = static_cast<unsigned char>(std::round((cosf(angle) * 0.5 + 0.5) * 255.0));
        const auto g = static_cast<unsigned char>(std::round((sinf(angle) * 0.5 + 0.5) * 255.0));
        const unsigned char b = 255 - r;
        const unsigned char a = 255 - g;
        kernelData.push_back(r);
        kernelData.push_back(g);
        kernelData.push_back(b);
        kernelData.push_back(a);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, kernelTextureSize, kernelTextureSize, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, kernelData.data());

    auto kernelTexture = CstTextureSampler_uptr(new TextureSampler());
    TextureSampler::setActiveTexture(kernelTextureIndex);
    kernelTexture->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return kernelTexture;
}

vecCstShaderProgram_sptr LevelProcess::getShaderPrograms() const {
    return {_mapShaderProgram, _starShaderProgram};
}
