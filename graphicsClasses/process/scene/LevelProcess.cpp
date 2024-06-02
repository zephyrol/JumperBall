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
                           CstTextureSampler_uptr depthKernel,
                           RenderGroup_sptr mapGroup,
                           ShaderProgram_sptr mapShaderProgram,
                           RenderGroup_sptr starGroup,
                           ShaderProgram_sptr starShaderProgram)
    : _width(width),
      _height(height),
      _firstShadow(std::move(firstShadow)),
      _secondShadow(std::move(secondShadow)),
      _levelFrameBuffer(std::move(levelFrameBuffer)),
      _depthKernel(std::move(depthKernel)),
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
        {{"shadowPixelsSize", shadowPixelsSize}},
        {{"fragCoordToKernelUv",
          {static_cast<float>(width) / static_cast<float>(kernelTextureSize),
           static_cast<float>(height) / static_cast<float>(kernelTextureSize)}}});

    shader->use();
    shader->setTextureIndex("depthTexture", firstShadowTextureIndex);
    shader->setTextureIndex("depth2Texture", secondShadowTextureIndex);
    shader->setTextureIndex("depthKernelTexture", kernelTextureIndex);
    return shader;
}

CstTextureSampler_uptr LevelProcess::createDepthKernel() {

    // Random kernel sample
    std::vector<unsigned char> kernelData{
        255, 128, 0,   127, 250, 161, 5,   94, 88,  249, 167, 6,   7,   170, 248, 85, 1,   140, 254, 115,
        233, 199, 22,  56,  122, 255, 133, 0,  168, 249, 87,  6,   214, 221, 41,  34, 39,  219, 216, 36,
        250, 163, 5,   92,  103, 253, 152, 2,  95,  251, 160, 4,   22,  199, 233, 56, 91,  250, 164, 5,
        123, 255, 132, 0,   9,   176, 246, 79, 0,   129, 255, 126, 44,  224, 211, 31, 1,   141, 254, 114,
        203, 230, 52,  25,  157, 252, 98,  3,  6,   167, 249, 88,  69,  241, 186, 14, 6,   166, 249, 89,
        1,   143, 254, 112, 238, 191, 17,  64, 12,  183, 243, 72,  5,   164, 250, 91, 204, 229, 51,  26,
        3,   153, 252, 102, 161, 250, 94,  5,  200, 233, 55,  22,  122, 255, 133, 0,  169, 248, 86,  7,
        27,  206, 228, 49,  56,  233, 199, 22, 110, 254, 145, 1,   91,  250, 164, 5,  250, 164, 5,   91,
        85,  248, 170, 7,   154, 252, 101, 3,  88,  249, 167, 6,   15,  187, 240, 68, 131, 255, 124, 0,
        31,  211, 224, 44,  252, 156, 3,   99, 233, 200, 22,  55,  225, 210, 30,  45, 29,  209, 226, 46,
        251, 161, 4,   94,  235, 196, 20,  59, 82,  247, 173, 8,   96,  251, 159, 4,  40,  221, 215, 34,
        198, 234, 57,  21,  152, 253, 103, 2,  162, 250, 93,  5,   5,   163, 250, 92, 0,   131, 255, 124,
        97,  251, 158, 4,   238, 190, 17,  65, 53,  231, 202, 24,  191, 238, 64,  17
    };
    auto kernelTexture = CstTextureSampler_uptr(new TextureSampler());
    TextureSampler::setActiveTexture(kernelTextureIndex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, kernelTextureSize, kernelTextureSize, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, kernelData.data());

    kernelTexture->bind();
    return kernelTexture;
}

vecCstShaderProgram_sptr LevelProcess::getShaderPrograms() const {
    return {_mapShaderProgram, _starShaderProgram};
}
