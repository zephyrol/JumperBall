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
    _uniformMatrix4(),
    _uniformVec4(),
    _uniformVec3(),
    _uniformVec2(),
    _uniformFloat(),
    _uniformBool(),
    //_quadFrame(),
    /*_meshMap(map),
       _meshBall(ball),
       _meshStar(star),
       _meshQuadFrame(_quadFrame),
       _meshMapUpdate([this] (size_t) {
       _meshMap.update();
       }),
       _meshBallUpdate([this] (size_t) {
       _meshBall.update();
       }),
       _meshStarUpdate([this] (size_t) {
       _meshStar.update();
       }),*/
    _uniformUpdate([this] (size_t) {
                       updateUniform();
                   }),
    _camera(camera),
    _spMap(Shader(GL_VERTEX_SHADER, vsshaderMap),
           Shader(GL_FRAGMENT_SHADER, fsshaderMap)),
    _spStar(Shader(GL_VERTEX_SHADER, vsshaderStar),
            Shader(GL_FRAGMENT_SHADER, fsshaderStar)),
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
        Shader(GL_FRAGMENT_SHADER, fsshaderDepth)),
    _light("light", _spMap,
           glm::vec3(0.f, 0.f, 0.f),
           glm::vec3(0.7f, 0.7f, 0.7f),
           glm::vec3(0.25f, 0.25f, 0.25f),
           glm::vec3(0.25f, 0.25f, 0.25f)),
    _frameBufferDepth(FrameBuffer::TextureCaterory::Depth,
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
                                   false),
    _mapState(map),
    _meshesMap(MeshGenerator::genMap(_mapState)),
    _renderPass(_spMap, _meshesMap)
{
    update();
}

void SceneRendering::phongEffect (GLuint depthTexture) const {

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.z, 0.0f);
    _frameBufferHDRScene.bindFrameBuffer(true);
    _spMap.use();
    _spMap.bindUniformTexture("depthTexture", 0, depthTexture);

    // --- Ball Map and Light ---
    bindCamera(_spMap);

    // Light
    _light.bind();

    // Ball
    // const BallState& ball = _meshBall.getInstanceFrame();
    // _spMap.bindUniform("burningCoeff", ball.burnCoefficient());
    // _meshBall.render(_spMap);

    // Map
    _spMap.bindUniform("burningCoeff", 0.f);
    // deprecated _meshMap.render(_spMap);

    // ------ Star ------
    // const StarState& star = _meshStar.getInstanceFrame();
    /*_spStar.use();
       _spStar.bindUniform("radiusInside", star.radiusInside());
       _spStar.bindUniform("radiusOutside", star.radiusOutside());
       _spStar.bindUniform("colorInside", star.colorInside());
       _spStar.bindUniform("colorOutside", star.colorOutside());*/

    bindCamera(_spStar);

    // _meshStar.render(_spStar);

}

void SceneRendering::blurEffect (GLuint brightPassTexture) const {

    _spBlur.use();
    _frameBufferHalfBlurEffect.bindFrameBuffer(false);

    _spBlur.bindUniformTexture("frameTexture", 0, brightPassTexture);
    _spBlur.bindUniform("patchSize", static_cast <int>(blurPatchSize));
    _spBlur.bindUniform("gaussWeights", gaussComputedValues);

    _spBlur.bindUniform("firstPass", true);
    // deprecated _meshQuadFrame.render(_spBlur);

    _frameBufferCompleteBlurEffect.bindFrameBuffer(false);
    _spBlur.bindUniformTexture("frameTexture", 0,
                               _frameBufferHalfBlurEffect.getRenderTexture());

    _spBlur.bindUniform("firstPass", false);
    // _meshQuadFrame.render(_spBlur);
}

void SceneRendering::brightPassEffect (GLuint hdrSceneTexture) const {
    constexpr float bloomThreshold = 4.f;
    _spBrightPassFilter.use();
    _frameBufferBrightPassEffect.bindFrameBuffer(false);

    _spBrightPassFilter.bindUniformTexture("frameTexture", 0, hdrSceneTexture);
    _spBrightPassFilter.bindUniform("threshold", bloomThreshold);
    // deprecated _meshQuadFrame.render(_spBrightPassFilter);
}

void SceneRendering::bloomEffect (GLuint hdrSceneTexture,
                                  GLuint bluredTexture) const {
    _spBloom.use();
    FrameBuffer::bindDefaultFrameBuffer();

    _spBloom.bindUniformTexture("frameSceneHDRTexture", 0, hdrSceneTexture);
    _spBloom.bindUniformTexture("frameBluredTexture", 1, bluredTexture);

    _spBloom.bindUniform("averageLuminance", 1.8f);
    _spBloom.bindUniform("whiteLuminance", 0.f);

    // Post process effects
    // const BallState& ball = _meshBall.getInstanceFrame();
    // _spBloom.bindUniform("teleportationCoefficient", ball.teleportationCoeff());
    // _spBloom.bindUniform(
    // "flashColor", Utility::colorAsVec3(ball.teleportationColor())
    // );

    // deprecated _meshQuadFrame.render(_spBloom);
}

void SceneRendering::depthFromStar() const {

    glClearColor(1.f, 1.f, 1.f, 0.0f);
    _frameBufferDepth.bindFrameBuffer(true);
    _spDepth.use();
    bindCamera(_spDepth);
    // depc _meshBall.render(_spDepth);
    // dpec _meshMap.render(_spDepth);
    // _meshQuadFrame.render(_spDepth);
}


void SceneRendering::bindCamera (const ShaderProgram& sp) const {
    //sp.bindUniform("VP", _uniformMatrix4.at("VP"));
    //sp.bindUniform("VPStar", _uniformMatrix4.at("VPStar"));
    //sp.bindUniform("positionBall", _uniformVec3.at("positionBall"));
    //sp.bindUniform("positionCamera", _uniformVec3.at("positionCamera"));
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
       halfBoundingBoxSize;

       const JBTypes::vec3f& positionBall = ball.get3DPosition();
       _light.directionLight(glm::normalize(center - star.centralPosition()));

       _uniformMatrix4["VPStar"] =
       glm::mat4(glm::ortho(-halfBoundingBoxSize,
       halfBoundingBoxSize,
       -halfBoundingBoxSize, halfBoundingBoxSize,
       _camera.zNear, _camera.zFar) *
       glm::lookAt(closeStarPosition, center, glm::vec3(0.f, 1.f, 0.f)));

       // uniform for rendering from camera
       _uniformMatrix4["VP"] = _camera.viewProjection();


       _uniformVec3["positionBall"] = glm::vec3(positionBall.x,
       positionBall.y,
       positionBall.z);

       _uniformVec3["positionCamera"] = _camera.pos();*/
    // uniform for Star View SceneRendering
    _light.update();
}

void SceneRendering::updateCamera(RenderPass& renderPass) {
    renderPass.upsertUniform("VP", _uniformMatrix4.at("VP"));
    renderPass.upsertUniform("VPStar", _uniformMatrix4.at("VPStar"));
    renderPass.upsertUniform("positionBall", _uniformVec3.at("positionBall"));
    renderPass.upsertUniform("positionCamera", _uniformVec3.at("positionCamera"));
}

void SceneRendering::render() const {

    // alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // -----
    glEnable(GL_CULL_FACE);

    depthFromStar();

    phongEffect(_frameBufferDepth.getRenderTexture());

    brightPassEffect(_frameBufferHDRScene.getRenderTexture());

    blurEffect(_frameBufferBrightPassEffect.getRenderTexture());

    bloomEffect(_frameBufferHDRScene.getRenderTexture(),
                _frameBufferCompleteBlurEffect.getRenderTexture());

    /*_spFbo.use();
       FrameBuffer::bindDefaultFrameBuffer();
       _spFbo.bindUniformTexture("frameTexture", 0,
       _frameBufferDepth.getRenderTexture());
       _meshQuadFrame.render(_spFbo);*/

}


void SceneRendering::update() {

    _mapState.update();
    _renderPass.update();
    // Update meshes and uniform values using multithreading
    // _meshMapUpdate.runTasks();
    // _meshBallUpdate.runTasks();
    // _meshStarUpdate.runTasks();
    // Wait the end of updates
    // _meshStarUpdate.waitTasks();
    // _meshBallUpdate.waitTasks();
    // _meshMapUpdate.waitTasks();

    // updateUniform();
}

const std::string SceneRendering::vsshaderMap = "shaders/phongVs.vs";
const std::string SceneRendering::fsshaderMap = "shaders/phongFs.fs";

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
const std::string SceneRendering::fsshaderDepth = "shaders/depthFs.fs";

const std::vector <float> SceneRendering::gaussComputedValues =
    Utility::genGaussBuffer(
        SceneRendering::blurPatchSize, SceneRendering::blurSigma);

const glm::vec3 SceneRendering::backgroundColor { 0.f, 0.f, .1f };
