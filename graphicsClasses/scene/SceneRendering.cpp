/*
 * File: SceneRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#include "SceneRendering.h"
#include <future>

SceneRendering::SceneRendering(const Map& map,
                               const Ball& ball,
                               const Star& star,
                               const Camera& camera):
    _quadScreen(),
    _camera(camera),
    _light(std::make_shared <UniformLight>("light",
                                           glm::vec3(0.f, 0.f, 0.f),
                                           glm::vec3(0.7f, 0.7f, 0.7f),
                                           glm::vec3(0.25f, 0.25f, 0.25f),
                                           glm::vec3(0.25f, 0.25f, 0.25f))),
    _starState(star),
    _VPStar(genVPStar()),
    _renderPasses{
                  std::make_shared <RenderPass>(MeshGenerator::genBlocks(map)),
                  std::make_shared <RenderPass>(MeshGenerator::genObjects(map)),
                  std::make_shared <RenderPass>(MeshGenerator::genEnemies(map)),
                  std::make_shared <RenderPass>(MeshGenerator::genBall(ball)),
                  std::make_shared <RenderPass>(MeshGenerator::genStar(star)),
                  std::make_shared <RenderPass>(MeshGenerator::genQuad(_quadScreen))
                  },
    _depthStarProcess(std::make_shared <RenderProcess>(
                          getLevelRenderPasses(),
                          createDepthStarShaders(),
                          createDepthStarUniforms(),
                          FrameBuffer_uptr(new FrameBuffer(
                                               FrameBuffer::TextureCaterory::Depth,
                                               true,
                                               sizeDepthTexture,
                                               sizeDepthTexture,
                                               true,
                                               { 1.f, 1.f, 1.f }))
                          )
                      ),
    _sceneRenderingProcess(std::make_shared <RenderProcess>(
                               getSceneRenderPasses(),
                               createSceneRenderingShaders(),
                               createSceneRenderingUniforms(),
                               FrameBuffer_uptr(new FrameBuffer(FrameBuffer::TextureCaterory::HDR, true))
                               )),
    _brightPassFilterProcess(std::make_shared <RenderProcess>(
                                 vecScreenRenderPass(),
                                 createBrightPassShaders(),
                                 createBrightPassUniforms(),
                                 createBloomEffectFrameBuffer(FrameBuffer::TextureCaterory::HDR)
                                 )),
    _horizontalBlurProcess(std::make_shared <RenderProcess>(
                               vecScreenRenderPass(),
                               createHorizontalBlurShaders(),
                               createHorizontalBlurUniforms(),
                               createBloomEffectFrameBuffer(FrameBuffer::TextureCaterory::HDR)
                               )),
    _verticalBlurProcess(std::make_shared <RenderProcess>(
                             vecScreenRenderPass(),
                             createVerticalBlurShaders(),
                             createVerticalBlurUniforms(),
                             createBloomEffectFrameBuffer(FrameBuffer::TextureCaterory::SDR)
                             )),
    _bloomProcess(std::make_shared <RenderProcess>(
                      vecScreenRenderPass(),
                      createBloomShaders(),
                      createBloomUniforms(),
                      nullptr
                      )),
    _sceneRenderingPipeline{
                            _depthStarProcess,
                            _sceneRenderingProcess,
                            _brightPassFilterProcess,
                            _horizontalBlurProcess,
                            _verticalBlurProcess,
                            _bloomProcess
                            } {
    update();

    // alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // -----
    glEnable(GL_CULL_FACE);
}

RenderProcess::PassShaderMap SceneRendering::createDepthStarShaders() const {
    RenderProcess::PassShaderMap depthStarRenderingShaders;

    const vecRenderPass_sptr levelRenderPasses = getLevelRenderPasses();
    depthStarRenderingShaders[levelRenderPasses.at(0)] =
        ShaderProgram::createShaderProgram(blocksVs, depthFs);
    depthStarRenderingShaders[levelRenderPasses.at(1)] =
        ShaderProgram::createShaderProgram(objectsMapVs, depthFs);
    depthStarRenderingShaders[levelRenderPasses.at(2)] =
        ShaderProgram::createShaderProgram(enemiesVs, depthFs);
    depthStarRenderingShaders[levelRenderPasses.at(3)] = ShaderProgram::createShaderProgram(ballVs, depthFs);
    return depthStarRenderingShaders;
}


RenderProcess::PassShaderMap SceneRendering::createSceneRenderingShaders() const {

    RenderProcess::PassShaderMap sceneRenderingShaders;
    const auto fillSceneRenderingShader =
        [this, &sceneRenderingShaders] ( const RenderPass_sptr& renderPass,
                                         const std::string& vertexShader,
                                         const std::string& fragmentShader,
                                         bool useLight)->void {
            CstShaderProgram_uptr shaderProgram = ShaderProgram::createShaderProgram(vertexShader,
                                                                                     fragmentShader);
            if (useLight) {
                _light->registerShader(shaderProgram);
            }
            sceneRenderingShaders[renderPass] = std::move(shaderProgram);
        };

    const vecRenderPass_sptr levelRenderPasses = getLevelRenderPasses();
    fillSceneRenderingShader(levelRenderPasses.at(0), blocksVs, levelFs, true);
    fillSceneRenderingShader(levelRenderPasses.at(1), objectsMapVs, levelFs, true);
    fillSceneRenderingShader(levelRenderPasses.at(2), enemiesVs, levelFs, true);
    fillSceneRenderingShader(levelRenderPasses.at(3), ballVs, levelFs, true);

    fillSceneRenderingShader(getStarRenderPass(), "starVs.vs", "starFs.fs", false);
    return sceneRenderingShaders;
}

const RenderPass_sptr& SceneRendering::getScreenRenderPass() const {
    return _renderPasses.at(5);
}

std::vector <RenderPass_sptr> SceneRendering::vecScreenRenderPass() const {
    return { getScreenRenderPass() };
}

vecRenderPass_sptr SceneRendering::getLevelRenderPasses() const {
    return { _renderPasses.at(0), _renderPasses.at(1), _renderPasses.at(2), _renderPasses.at(3) };
}

const RenderPass_sptr& SceneRendering::getStarRenderPass() const {
    return _renderPasses.at(4);
}

vecRenderPass_sptr SceneRendering::getSceneRenderPasses() const {
    vecRenderPass_sptr sceneRenderPasses = getLevelRenderPasses();
    sceneRenderPasses.push_back(getStarRenderPass());
    return sceneRenderPasses;
}

RenderProcess::PassShaderMap SceneRendering::createScreenShaders (
    const std::string& vs,
    const std::string& fs
    ) const {
    RenderProcess::PassShaderMap screenEffectShaders;
    screenEffectShaders[getScreenRenderPass()] = ShaderProgram::createShaderProgram(vs, fs);
    return screenEffectShaders;
}

RenderProcess::PassShaderMap SceneRendering::createBrightPassShaders() const {
    return createScreenShaders(basicFboVs, "brightPassFilter.fs");
}

RenderProcess::PassShaderMap SceneRendering::createHorizontalBlurShaders() const {
    return createScreenShaders(basicFboVs, "horizontalBlurFs.fs");
}

RenderProcess::PassShaderMap SceneRendering::createVerticalBlurShaders() const {
    return createScreenShaders(basicFboVs, "verticalBlurFs.fs");
}

RenderProcess::PassShaderMap SceneRendering::createBloomShaders() const {
    return createScreenShaders(basicFboVs, "bloomFs.fs");
}


RenderProcess::PassUniformUpdateMap SceneRendering::createDepthStarUniforms() const {

    RenderProcess::PassUniformUpdateMap uniformUpdatingFcts;

    const auto updateDepthStar =
        [this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
            this->updateDepthUniforms(renderPass, shaderProgramID);
        };
    for (const RenderPass_sptr& levelRenderPass : getLevelRenderPasses()) {
        uniformUpdatingFcts[levelRenderPass] = updateDepthStar;
    }
    return uniformUpdatingFcts;
}

RenderProcess::PassUniformUpdateMap SceneRendering::createSceneRenderingUniforms() const {

    RenderProcess::PassUniformUpdateMap uniformUpdatingFcts;
    const auto updateCam =
        [this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
            this->updateCameraUniforms(renderPass, shaderProgramID);
        };
    const auto updateCamStar =
        [this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
            this->updateCameraUniformsStar(renderPass, shaderProgramID);
        };

    for (const RenderPass_sptr& levelRenderPass : getLevelRenderPasses()) {
        uniformUpdatingFcts[levelRenderPass] = updateCam;
    }
    uniformUpdatingFcts[getStarRenderPass()] = updateCamStar;
    return uniformUpdatingFcts;
}

RenderProcess::PassUniformUpdateMap SceneRendering::createScreenUniforms (
    const std::function <void(const RenderPass_sptr&, GLuint)>& uniformsUpdatingFunction
    ) const {
    RenderProcess::PassUniformUpdateMap uniformUpdatingFcts;
    uniformUpdatingFcts[getScreenRenderPass()] = uniformsUpdatingFunction;
    return uniformUpdatingFcts;
}


RenderProcess::PassUniformUpdateMap SceneRendering::createBrightPassUniforms() const {
    return createScreenUniforms([this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
                                    this->updateBrightPassFilterUniforms(renderPass, shaderProgramID);
                                });
}

RenderProcess::PassUniformUpdateMap SceneRendering::createHorizontalBlurUniforms() const {
    return createScreenUniforms([this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
                                    this->updateHorizontalBlurUniforms(renderPass, shaderProgramID);
                                });
}

RenderProcess::PassUniformUpdateMap SceneRendering::createVerticalBlurUniforms() const {
    return createScreenUniforms([this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
                                    this->updateVerticalBlurUniforms(renderPass, shaderProgramID);
                                });
}

RenderProcess::PassUniformUpdateMap SceneRendering::createBloomUniforms() const {
    return createScreenUniforms([this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
                                    this->updateBloomUniforms(renderPass, shaderProgramID);
                                });
}

void SceneRendering::updateDepthUniforms (const RenderPass_sptr& renderPass, GLuint shaderProgramID) const {
    renderPass->upsertUniform(shaderProgramID, "VP", _VPStar);
}

void SceneRendering::updateCameraUniforms (const RenderPass_sptr& renderPass, GLuint shaderProgramID) const {
    renderPass->upsertUniform(shaderProgramID, "VP", _camera.viewProjection());
    renderPass->upsertUniform(shaderProgramID, "VPStar", _VPStar);
    renderPass->upsertUniform(shaderProgramID, "positionCamera", _camera.pos());

    renderPass->upsertUniform(shaderProgramID, _light->name(), _light);
    renderPass->upsertUniformTexture(
        shaderProgramID,
        "depthTexture",
        _depthStarProcess->getFrameBufferTexture()
        );
}

void SceneRendering::updateCameraUniformsStar (
    const RenderPass_sptr& renderPass,
    GLuint shaderProgramID) const {
    renderPass->upsertUniform(shaderProgramID, "VP", _camera.viewProjection());
    renderPass->upsertUniform(shaderProgramID, "positionCamera", _camera.pos());
}

void SceneRendering::updateBrightPassFilterUniforms (
    const RenderPass_sptr& renderPass,
    GLuint shaderProgramID) const {
    constexpr float brightPassThreshold = 4.f;
    renderPass->upsertUniformTexture(
        shaderProgramID,
        "textureScene",
        _sceneRenderingProcess->getFrameBufferTexture()
        );
    renderPass->upsertUniform(shaderProgramID, "threshold", brightPassThreshold);
}

void SceneRendering::updateHorizontalBlurUniforms (
    const RenderPass_sptr& renderPass,
    GLuint shaderProgramID) const {
    renderPass->upsertUniformTexture(
        shaderProgramID,
        "brightPassTexture",
        _brightPassFilterProcess->getFrameBufferTexture()
        );
}

void SceneRendering::updateVerticalBlurUniforms (
    const RenderPass_sptr& renderPass,
    GLuint shaderProgramID) const {
    renderPass->upsertUniformTexture(
        shaderProgramID,
        "horizontalBlurTexture",
        _horizontalBlurProcess->getFrameBufferTexture()
        );
}

void SceneRendering::updateBloomUniforms (const RenderPass_sptr& renderPass, GLuint shaderProgramID) const {
    renderPass->upsertUniformTexture(
        shaderProgramID,
        "frameSceneHDRTexture",
        _sceneRenderingProcess->getFrameBufferTexture()
        );
    renderPass->upsertUniformTexture(
        shaderProgramID,
        "frameBluredTexture",
        _verticalBlurProcess->getFrameBufferTexture()
        );
    renderPass->upsertUniform(shaderProgramID, "averageLuminance", 1.8f);

}

glm::mat4 SceneRendering::genVPStar() const {

    constexpr float offsetJumpingBall = 1.f; // size of ball + jump height
    const float halfBoundingBoxSize = _starState.envSize() / 2.f + offsetJumpingBall;

    // We use a close star position to get a better ZBuffer accuracy
    const glm::vec3 centerWorld = Utility::convertToOpenGLFormat(_starState.rotationCenter());
    const glm::vec3 closeStarPosition = centerWorld +
                                        glm::normalize((Utility::convertToOpenGLFormat(_starState.position())
                                                        - centerWorld)) *
                                        halfBoundingBoxSize;

    return glm::ortho(
        -halfBoundingBoxSize,
        halfBoundingBoxSize,
        -halfBoundingBoxSize,
        halfBoundingBoxSize,
        _camera.zNear, _camera.zNear + 2.f * halfBoundingBoxSize
        ) * glm::lookAt(closeStarPosition, centerWorld, glm::vec3(0.f, 1.f, 0.f));
}

FrameBuffer_uptr SceneRendering::createBloomEffectFrameBuffer (
    const FrameBuffer::TextureCaterory& category
    ) const {
    return FrameBuffer_uptr(new FrameBuffer(
                                category,
                                false,
                                Utility::getWidthFromHeight(heightBloomTexture),
                                heightBloomTexture,
                                false
                                ));
}

void SceneRendering::update() {
    _starState.update();
    _VPStar = genVPStar();
    _light->directionLight(Utility::convertToOpenGLFormat(_starState.lightDirection()));
    _light->update();

    for (auto& renderPass : _renderPasses) {
        renderPass->update();
    }
    for (auto& renderProcess : _sceneRenderingPipeline) {
        renderProcess->updateUniforms();
    }
}

void SceneRendering::render() const {

    for (const auto& renderProcess : _sceneRenderingPipeline) {
        renderProcess->render();
    }

}

const std::string SceneRendering::blocksVs = "blocksVs.vs";
const std::string SceneRendering::objectsMapVs = "objectsMapVs.vs";
const std::string SceneRendering::enemiesVs = "enemiesVs.vs";
const std::string SceneRendering::ballVs = "ballVs.vs";
const std::string SceneRendering::basicFboVs = "basicFboVs.vs";
const std::string SceneRendering::levelFs = "levelFs.fs";
const std::string SceneRendering::depthFs = "depthFs.fs";
