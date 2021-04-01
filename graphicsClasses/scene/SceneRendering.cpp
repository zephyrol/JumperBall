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
    /*_uniformMatrix4(),
       _uniformVec4(),
       _uniformVec3(),
       _uniformVec2(),
       _uniformFloat(),
       _uniformBool(),*/
    _quadScreen(),
    _camera(camera),
    /*_spBlocks(Shader(GL_VERTEX_SHADER, vsshaderBlocks),
              Shader(GL_FRAGMENT_SHADER, fsshaderBlocks)),
       _spObjects(Shader(GL_VERTEX_SHADER, vsshaderObjects),
               Shader(GL_FRAGMENT_SHADER, fsshaderObjects)),
       _spStar(Shader(GL_VERTEX_SHADER, vsshaderStar),
            Shader(GL_FRAGMENT_SHADER, fsshaderStar)),
       _spBall(Shader(GL_VERTEX_SHADER, vsshaderBall),
            Shader(GL_FRAGMENT_SHADER, fsshaderBall)),
       _spFbo(Shader(GL_VERTEX_SHADER, vsshaderFBO),
           Shader(GL_FRAGMENT_SHADER, fsshaderFBO)),
       _spBlur(Shader(GL_VERTEX_SHADER, vsshaderBlur),
            Shader(GL_FRAGMENT_SHADER, fsshaderBlur)),
       _spBrightPassFilter
        (Shader(GL_VERTEX_SHADER, vsshaderBrightPassFilter),
        Shader(GL_FRAGMENT_SHADER, fsshaderBrightPassFilter)),
       _spBloom
        (Shader(GL_VERTEX_SHADER, vsshaderBloom),
        Shader(GL_FRAGMENT_SHADER, fsshaderBloom)),
       _spDepth
        (Shader(GL_VERTEX_SHADER, vsshaderDepth),
        Shader(GL_FRAGMENT_SHADER, fsshaderDepth)),*/
    _light(std::make_shared <UniformLight>("light",
                                           glm::vec3(0.f, 0.f, 0.f),
                                           glm::vec3(0.7f, 0.7f, 0.7f),
                                           glm::vec3(0.25f, 0.25f, 0.25f),
                                           glm::vec3(0.25f, 0.25f, 0.25f))),
    /*_frameBufferDepth(FrameBuffer::TextureCaterory::Depth,
                      sizeDepthTexture, sizeDepthTexture),
       _frameBufferHDRScene(FrameBuffer::TextureCaterory::HDR),
       _frameBufferBrightPassEffect(FrameBuffer::TextureCaterory::HDR,
                                 Utility::getWidthFromHeight(heightBloomTexture),
                                 heightBloomTexture,
                                 false),
       _frameBufferHalfBlurEffect(FrameBuffer::TextureCaterory::HDR,
                               Utility::getWidthFromHeight(heightBloomTexture),
                               heightBloomTexture,
                               false),
       _frameBufferCompleteBlurEffect(FrameBuffer::TextureCaterory::SDR,
                                   Utility::getWidthFromHeight(heightBloomTexture),
                                   heightBloomTexture,
                                   false),*/
    // _mapState(map),
    // _renderPassBlocks(_spBlocks, MeshGenerator::genBlocks(map)),
    // _renderPassObjects(_spObjects, MeshGenerator::genObjects(map)),
    _renderPasses{
                  std::make_shared <RenderPass>(MeshGenerator::genBlocks(map)),
                  std::make_shared <RenderPass>(MeshGenerator::genObjects(map)),
                  std::make_shared <RenderPass>(MeshGenerator::genBall(ball)),
                  std::make_shared <RenderPass>(MeshGenerator::genStar(star)),
                  std::make_shared <RenderPass>(MeshGenerator::genQuad(_quadScreen))
                  },
    _starState(star),
    _sceneRenderingProcess(std::make_shared <RenderProcess>(
                               getSceneRenderPasses(),
                               createSceneRenderingShaders(),
                               createSceneRenderingUniforms(),
                               // nullptr
                               FrameBuffer_uptr(new FrameBuffer(FrameBuffer::TextureCaterory::HDR))
                               )),
    _brightPassFilterProcess(std::make_shared <RenderProcess>(
                                 vecScreenRenderPass(),
                                 createBrightPassShaders(),
                                 createBrightPassUniforms(),
                                 createBloomEffectFrameBuffer(FrameBuffer::TextureCaterory::SDR)
                                 )),
    _horizontalBlurProcess(std::make_shared <RenderProcess>(
                               vecScreenRenderPass(),
                               createHorizontalBlurShaders(),
                               createHorizontalBlurUniforms(),
                               createBloomEffectFrameBuffer(FrameBuffer::TextureCaterory::SDR)
                               )),
    _verticalBlurProcess(std::make_shared <RenderProcess>(
                             vecScreenRenderPass(),
                             createVerticalBlurShaders(),
                             createVerticalBlurUniforms(),
                             nullptr // createBloomEffectFrameBuffer(FrameBuffer::TextureCaterory::SDR)
                             )),
    _sceneRenderingPipeline{
                            _sceneRenderingProcess,
                            _brightPassFilterProcess,
                            _horizontalBlurProcess,
                            _verticalBlurProcess
                            } {
    // _renderPassStar(_spStar, MeshGenerator::genStar(star)),
    // _renderPassBall(_spBall, MeshGenerator::genBall(ball))
    update();

    // alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // -----
    glEnable(GL_CULL_FACE);
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
    fillSceneRenderingShader(levelRenderPasses.at(0), "blocksVs.vs", "blocksFs.fs", true);
    fillSceneRenderingShader(levelRenderPasses.at(1), "objectsMapVs.vs", "blocksFs.fs", true);
    fillSceneRenderingShader(levelRenderPasses.at(2), "ballVs.vs", "blocksFs.fs", true);

    fillSceneRenderingShader(getStarRenderPass(), "starVs.vs", "starFs.fs", false);
    return sceneRenderingShaders;
}

const RenderPass_sptr& SceneRendering::getScreenRenderPass() const {
    return _renderPasses.at(4);
}

std::vector <RenderPass_sptr> SceneRendering::vecScreenRenderPass() const {
    return { getScreenRenderPass() };
}

vecRenderPass_sptr SceneRendering::getLevelRenderPasses() const {
    return { _renderPasses.at(0), _renderPasses.at(1), _renderPasses.at(2) };
}

const RenderPass_sptr& SceneRendering::getStarRenderPass() const {
    return _renderPasses.at(3);
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
    return createScreenShaders("basicFboVs.vs", "brightPassFilter.fs");
}

RenderProcess::PassShaderMap SceneRendering::createHorizontalBlurShaders() const {
    return createScreenShaders("basicFboVs.vs", "horizontalBlurFs.fs");
}

RenderProcess::PassShaderMap SceneRendering::createVerticalBlurShaders() const {
    return createScreenShaders("basicFboVs.vs", "verticalBlurFs.fs");
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


void SceneRendering::phongEffect (GLuint depthTexture) const {

    //    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.z, 0.0f);
    //    // _frameBufferHDRScene.bindFrameBuffer(true);
    //    FrameBuffer::cleanCurrentFrameBuffer(true);
    //    _spBlocks.use();
    //    _spBlocks.bindUniformTexture("depthTexture", 0, depthTexture);
    //
    //    // --- Ball Map and Light ---
    //    bindCamera(_spBlocks);
    //
    //    // Light
    //    // _light.bind();
    //
    //    // Ball
    //    // const BallState& ball = _meshBall.getInstanceFrame();
    //    // _spBlocks.bindUniform("burningCoeff", ball.burnCoefficient());
    //    // _meshBall.render(_spBlocks);
    //
    //    // Map
    //    _spBlocks.bindUniform("burningCoeff", 0.f);
    //    // deprecated _meshMap.render(_spBlocks);
    //
    //    // ------ Star ------
    //    // const StarState& star = _meshStar.getInstanceFrame();
    //    _spStar.use();
    //    /*_spStar.bindUniform("radiusInside", _starState.radiusInside());
    //       _spStar.bindUniform("radiusOutside", _starState.radiusOutside());
    //       _spStar.bindUniform("colorInside", _starState.colorInside());
    //       _spStar.bindUniform("colorOutside", _starState.colorOutside());*/
    //
    //    bindCamera(_spStar);
    //
    //    _renderPassBlocks.render();
    //    _renderPassObjects.render();
    //    _renderPassStar.render();
    //    _renderPassBall.render();
    //
    //    // _meshStar.render(_spStar);

}

void SceneRendering::blurEffect (GLuint brightPassTexture) const {

    //    _spBlur.use();
    //    _frameBufferHalfBlurEffect.bindFrameBuffer(false);
    //
    //    _spBlur.bindUniformTexture("frameTexture", 0, brightPassTexture);
    //    _spBlur.bindUniform("patchSize", static_cast <int>(blurPatchSize));
    //    _spBlur.bindUniform("gaussWeights", gaussComputedValues);
    //
    //    _spBlur.bindUniform("firstPass", true);
    //    // deprecated _meshQuadFrame.render(_spBlur);
    //
    //    _frameBufferCompleteBlurEffect.bindFrameBuffer(false);
    //    _spBlur.bindUniformTexture("frameTexture", 0,
    //                               _frameBufferHalfBlurEffect.getRenderTexture());
    //
    //    _spBlur.bindUniform("firstPass", false);
    //    // _meshQuadFrame.render(_spBlur);
}

void SceneRendering::brightPassEffect (GLuint hdrSceneTexture) const {
    //    constexpr float bloomThreshold = 4.f;
    //    _spBrightPassFilter.use();
    //    _frameBufferBrightPassEffect.bindFrameBuffer(false);
    //
    //    _spBrightPassFilter.bindUniformTexture("frameTexture", 0, hdrSceneTexture);
    //    _spBrightPassFilter.bindUniform("threshold", bloomThreshold);
    //    // deprecated _meshQuadFrame.render(_spBrightPassFilter);
}

void SceneRendering::bloomEffect (GLuint hdrSceneTexture,
                                  GLuint bluredTexture) const {
    //    _spBloom.use();
    //    FrameBuffer::bindDefaultFrameBuffer();
    //
    //    _spBloom.bindUniformTexture("frameSceneHDRTexture", 0, hdrSceneTexture);
    //    _spBloom.bindUniformTexture("frameBluredTexture", 1, bluredTexture);
    //
    //    _spBloom.bindUniform("averageLuminance", 1.8f);
    //    _spBloom.bindUniform("whiteLuminance", 0.f);
    //
    //    // Post process effects
    //    // const BallState& ball = _meshBall.getInstanceFrame();
    //    // _spBloom.bindUniform("teleportationCoefficient", ball.teleportationCoeff());
    //    // _spBloom.bindUniform(
    //    // "flashColor", Utility::colorAsVec3(ball.teleportationColor())
    //    // );
    //
    //    // deprecated _meshQuadFrame.render(_spBloom);
}

void SceneRendering::depthFromStar() const {
    //
    //    glClearColor(1.f, 1.f, 1.f, 0.0f);
    //    _frameBufferDepth.bindFrameBuffer(true);
    //    _spDepth.use();
    //    bindCamera(_spDepth);
    //    // depc _meshBall.render(_spDepth);
    //    // dpec _meshMap.render(_spDepth);
    //    // _meshQuadFrame.render(_spDepth);
}


void SceneRendering::bindCamera (const ShaderProgram& sp) const {
    // sp.bindUniform("VP", _uniformMatrix4.at("VP"));
    // sp.bindUniform("VPStar", _uniformMatrix4.at("VPStar"));
    // sp.bindUniform("positionBall", _uniformVec3.at("positionBall"));
    // sp.bindUniform("positionCamera", _uniformVec3.at("positionCamera"));
}

void SceneRendering::updateUniform() {
    // uniform for rendering from star

    /*const MapState& map = _meshMap.getInstanceFrame();
       const BallState& ball = _meshBall.getInstanceFrame();
       const StarState& star = _meshStar.getInstanceFrame();*/

    /*const glm::vec3 center { map.width() / 2.f,
       map.height() / 2.f,
       map.deep() / 2.f };

       float boundingBoxMax = static_cast <float>(map.width());
       if (boundingBoxMax < static_cast <float>(map.height()))
       boundingBoxMax = static_cast <float>(map.height());
       if (boundingBoxMax < static_cast <float>(map.deep()))
       boundingBoxMax = static_cast <float>(map.deep());

       constexpr float offsetJumpingBall = 1.f; // size of ball + jump height
       // float halfBoundingBoxSize = std::move(boundingBoxMax);
       // halfBoundingBoxSize = halfBoundingBoxSize/2.f + offsetJumpingBall;
       const float halfBoundingBoxSize = boundingBoxMax / 2.f + offsetJumpingBall;

       // We use a close star position to get a better ZBuffer accuracy
       const glm::vec3 closeStarPosition = center +
       glm::normalize((star.centralPosition() - center)) *
       halfBoundingBoxSize;*/

    /*const JBTypes::vec3f& positionBall = ball.get3DPosition();
       _light.directionLight(glm::normalize(center - star.centralPosition()));

       _uniformMatrix4["VPStar"] =
       glm::mat4(glm::ortho(-halfBoundingBoxSize,
       halfBoundingBoxSize,
       -halfBoundingBoxSize, halfBoundingBoxSize,
       _camera.zNear, _camera.zFar) *
       glm::lookAt(closeStarPosition, center, glm::vec3(0.f, 1.f, 0.f)));*/

    // uniform for rendering from camera
    /*_uniformMatrix4["VP"] = _camera.viewProjection();


       _uniformVec3["positionBall"] = glm::vec3(positionBall.x,
       positionBall.y,
       positionBall.z);

       _uniformVec3["positionCamera"] = _camera.pos();*/
    // uniform for Star View SceneRendering
    // _light.update();
}

void SceneRendering::updateCameraUniforms (const RenderPass_sptr& renderPass, GLuint shaderProgramID) const {
    // renderPass.upsertUniform("VP", _uniformMatrix4.at("VP"));
    renderPass->upsertUniform(shaderProgramID, "VP", _camera.viewProjection());
    // renderPass.upsertUniform("VPStar", _uniformMatrix4.at("VPStar"));
    // renderPass.upsertUniform("positionBall", _uniformVec3.at("positionBall"));
    renderPass->upsertUniform(shaderProgramID, "positionCamera", _camera.pos());

    renderPass->upsertUniform(shaderProgramID, _light->name(), _light);
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
    renderPass->upsertUniformTexture(shaderProgramID,
                                     "horizontalBlurTexture",
                                     _horizontalBlurProcess->getFrameBufferTexture()
                                     );
}

FrameBuffer_uptr SceneRendering::createBloomEffectFrameBuffer (
    const FrameBuffer::TextureCaterory& category
    ) const {
    return FrameBuffer_uptr(
        new FrameBuffer(
            category, Utility::getWidthFromHeight(heightBloomTexture), heightBloomTexture, false
            )
        );
}


void SceneRendering::update() {

    _starState.update();
    _light->directionLight(Utility::convertToOpenGLFormat(_starState.lightDirection()));
    _light->update();

    for (auto& renderPass : _renderPasses) {
        renderPass->update();
    }

    for (auto& renderProcess : _sceneRenderingPipeline) {
        renderProcess->updateUniforms();
    }

    //    _renderPassBlocks.update();
    //    _renderPassObjects.update();
    //    _renderPassStar.update();
    //    _renderPassBall.update();
    //    updateCamera(_renderPassBlocks);
    //    updateCamera(_renderPassObjects);
    //    updateCamera(_renderPassStar);
    //    updateCamera(_renderPassBall);
    //    // Update meshes and uniform values using multithreading
    //    // _meshMapUpdate.runTasks();
    //    // _meshBallUpdate.runTasks();
    //    // _meshStarUpdate.runTasks();
    //    // Wait the end of updates
    //    // _meshStarUpdate.waitTasks();
    //    // _meshBallUpdate.waitTasks();
    //    // _meshMapUpdate.waitTasks();

    //    updateUniform();
}


void SceneRendering::render() const {

    for (const auto& renderProcess : _sceneRenderingPipeline) {
        renderProcess->render();
    }

    // depthFromStar();

    // phongEffect(_frameBufferDepth.getRenderTexture());

    // brightPassEffect(_frameBufferHDRScene.getRenderTexture());

    // blurEffect(_frameBufferBrightPassEffect.getRenderTexture());

    // bloomEffect(_frameBufferHDRScene.getRenderTexture(),
    //            _frameBufferCompleteBlurEffect.getRenderTexture());

    /*_spFbo.use();
       FrameBuffer::bindDefaultFrameBuffer();
       _spFbo.bindUniformTexture("frameTexture", 0,
       _frameBufferDepth.getRenderTexture());
       _meshQuadFrame.render(_spFbo);*/

}

/*const std::string SceneRendering::vsshaderBlocks = "shaders/blocksVs.vs";
   const std::string SceneRendering::fsshaderBlocks = "shaders/blocksFs.fs";

   const std::string SceneRendering::vsshaderObjects = "shaders/objectsMapVs.vs";
   const std::string SceneRendering::fsshaderObjects = "shaders/blocksFs.fs";

   const std::string SceneRendering::vsshaderBall = "shaders/ballVs.vs";
   const std::string SceneRendering::fsshaderBall = "shaders/blocksFs.fs";

   const std::string SceneRendering::vsshaderStar = "shaders/starVs.vs";
   const std::string SceneRendering::fsshaderStar = "shaders/starFs.fs";

   const std::string SceneRendering::vsshaderFBO = "shaders/basicFboVs.vs";
   const std::string SceneRendering::fsshaderFBO = "shaders/basicFboFs.fs";

   const std::string SceneRendering::vsshaderBlur = "shaders/basicFboVs.vs";
   const std::string SceneRendering::fsshaderBlur = "shaders/blurFs.fs";

   const std::string SceneRendering::vsshaderBrightPassFilter =
    "shaders/basicFboVs.vs";
   const std::string SceneRendering::fsshaderBrightPassFilter =
    "shaders/brightPassFilter.fs";

   const std::string SceneRendering::vsshaderBloom = "shaders/basicFboVs.vs";
   const std::string SceneRendering::fsshaderBloom = "shaders/bloomFs.fs";
   const std::string SceneRendering::vsshaderDepth = "shaders/depthVs.vs";
   const std::string SceneRendering::fsshaderDepth = "shaders/depthFs.fs";*/
