/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Rendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#include "Rendering.h"
#include <future>

Rendering::Rendering(const Map&     map,
                     const Ball&    ball,
                     const Star&    star,
                     const Camera&  camera):
    _uniformMatrix4(),
    _uniformVec4(),
    _uniformVec3(),
    _uniformVec2(),
    _uniformFloat(),
    _uniformBool(),
    _quadFrame(),
    _meshMap(map),
    _meshBall(ball),
    _meshStar(star),
    _meshQuadFrame(_quadFrame),
    _meshMapUpdate([this](size_t) {
        _meshMap.update(); return 1;}),
    _meshBallUpdate([this](size_t) {
        _meshBall.update(); return 1;}),
    _meshStarUpdate([this](size_t) {
        _meshStar.update(); return 1; }),
    _ball(ball),
    _star(star),
    _camera(camera),
    _light( glm::vec3(0.f,0.f,0.f),
            glm::vec3(0.7f,0.7f,0.7f),
            glm::vec3(0.25f,0.25f,0.25f),
            glm::vec3(0.25f,0.25f,0.25f)),
    _spMap( Shader (GL_VERTEX_SHADER,   vsshaderMap ),
            Shader (GL_FRAGMENT_SHADER, fsshaderMap )),
    _spStar(Shader (GL_VERTEX_SHADER,   vsshaderStar),
            Shader (GL_FRAGMENT_SHADER, fsshaderStar)),
    _spFbo( Shader (GL_VERTEX_SHADER,   vsshaderFBO ),
            Shader (GL_FRAGMENT_SHADER, fsshaderFBO )),
    _spBlur(Shader (GL_VERTEX_SHADER,   vsshaderBlur),
            Shader (GL_FRAGMENT_SHADER, fsshaderBlur)),
    _spBrightPassFilter
          ( Shader (GL_VERTEX_SHADER,   vsshaderBrightPassFilter),
            Shader (GL_FRAGMENT_SHADER, fsshaderBrightPassFilter)),
    _spBloom
          ( Shader (GL_VERTEX_SHADER,   vsshaderBloom),
            Shader (GL_FRAGMENT_SHADER, fsshaderBloom)),
    _spDepth
          ( Shader (GL_VERTEX_SHADER,   vsshaderDepth),
            Shader (GL_FRAGMENT_SHADER, fsshaderDepth)),
    _frameBufferDepth(FrameBuffer::TextureCaterory::Depth,
                         sizeDepthTexture,sizeDepthTexture),
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
                        false)
{ }

void Rendering::phongEffect( GLuint depthTexture) {

    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.z, 0.0f);
    _frameBufferHDRScene.bindFrameBuffer(true);
    _spMap.use();
    _spMap.bindUniformTexture("depthTexture", 0, depthTexture);

    // --- Ball and Map ---
    bindCamera (_spMap);
    _light.positionLight(_star.centralPosition());
    _light.bind("light",_spMap);

    // Ball
    _spMap.bindUniform("burningCoeff", _ball.burnCoefficient());
    _meshBall.render(_spMap);

    // Map
    _spMap.bindUniform("burningCoeff", 0.f);
    _meshMap.render(_spMap);

    // ------ Star ------
    _spStar.use();

    _spStar.bindUniform("radiusInside",  _star.radiusInside());
    _spStar.bindUniform("radiusOutside", _star.radiusOutside());
    _spStar.bindUniform("colorInside",   _star.colorInside());
    _spStar.bindUniform("colorOutside",  _star.colorOutside());

    bindCamera(_spStar);

    _meshStar.render(_spStar);

}

void Rendering::blurEffect(GLuint brightPassTexture) const {

    _spBlur.use();
    _frameBufferHalfBlurEffect.bindFrameBuffer(false);

    _spBlur.bindUniformTexture("frameTexture", 0, brightPassTexture);
    _spBlur.bindUniform("patchSize", static_cast<int>(blurPatchSize));
    _spBlur.bindUniform("gaussWeights", gaussComputedValues);

    _spBlur.bindUniform("firstPass", true);
    _meshQuadFrame.render(_spBlur);

    _frameBufferCompleteBlurEffect.bindFrameBuffer(false);
    _spBlur.bindUniformTexture("frameTexture", 0,
                              _frameBufferHalfBlurEffect.getRenderTexture());

    _spBlur.bindUniform("firstPass", false);
    _meshQuadFrame.render(_spBlur);
}

void Rendering::brightPassEffect(GLuint hdrSceneTexture) const {
    constexpr float bloomThreshold = 4.f;
    _spBrightPassFilter.use();
    _frameBufferBrightPassEffect.bindFrameBuffer(false);

    _spBrightPassFilter.bindUniformTexture("frameTexture", 0, hdrSceneTexture);
    _spBrightPassFilter.bindUniform ("threshold", bloomThreshold);
    _meshQuadFrame.render(_spBrightPassFilter);
}

void Rendering::bloomEffect( GLuint hdrSceneTexture,
                             GLuint bluredTexture) const{
    _spBloom.use();
    FrameBuffer::bindDefaultFrameBuffer();

    _spBloom.bindUniformTexture("frameSceneHDRTexture", 0, hdrSceneTexture);
    _spBloom.bindUniformTexture("frameBluredTexture", 1, bluredTexture);

    _spBloom.bindUniform ( "averageLuminance", 1.8f);
    _spBloom.bindUniform ( "whiteLuminance", 0.f);
    _meshQuadFrame.render(_spBloom);
}

void Rendering::depthFromStar() {

    glClearColor(1.f, 1.f, 1.f, 0.0f);
    _frameBufferDepth.bindFrameBuffer(true);
    _spDepth.use();
    bindStarView(_spDepth);
    //bindCamera(_spDepth);
    //_spDepth.bindUniformTexture("frameTexture", 0);
    //_spDepth.bindUniform ("threshold",  5.f);
    _meshBall.render(_spDepth);
    _meshMap.render(_spDepth);
    //_meshQuadFrame.render(_spDepth);
}

void Rendering::render() {

    //alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //-----
    glEnable(GL_CULL_FACE);

    //Update meshes
    /*_meshMap.update();
    _meshBall.update();
    _meshStar.update();*/

    _meshStarUpdate.runTasks();
    _meshBallUpdate.runTasks();
    _meshMapUpdate.runTasks();

    _meshStarUpdate.waitTasks();
    _meshBallUpdate.waitTasks();
    _meshMapUpdate.waitTasks();


    depthFromStar();

    phongEffect(_frameBufferDepth.getRenderTexture());

    brightPassEffect(_frameBufferHDRScene.getRenderTexture());

    blurEffect(_frameBufferBrightPassEffect.getRenderTexture());

    bloomEffect(_frameBufferHDRScene.getRenderTexture(),
                _frameBufferCompleteBlurEffect.getRenderTexture());

    /*_spFbo.use();
    FrameBuffer::bindDefaultFrameBuffer();
    _spFbo.bindUniformTexture("frameTexture", 0,
                              _frameBufferHDRScene.getRenderTexture());
    _meshQuadFrame.render(_spFbo);*/

}

void Rendering::bindCamera(const ShaderProgram& sp) {

    _uniformMatrix4["VP"] = _camera.viewProjection();

    const JBTypes::vec3f& positionBall = _ball.get3DPosition();

    _uniformVec3["positionBall"]    = glm::vec3( positionBall.x,
                                                 positionBall.y,
                                                 positionBall.z);

    _uniformVec3["positionCamera"]  = _camera.pos();
    
    sp.bindUniform ("VP",             _uniformMatrix4.at("VP"));
    sp.bindUniform ("VPStar",         _uniformMatrix4.at("VPStar"));
    sp.bindUniform ("positionBall",   _uniformVec3.at("positionBall"));
    sp.bindUniform ("positionCamera", _uniformVec3.at("positionCamera"));
}

void Rendering::bindStarView(const ShaderProgram& sp) {

    const Map& map = _meshMap.base();

    const glm::vec3 center{ map.width()/2.f,
                            map.height()/2.f,
                            map.deep()/2.f };

    float boundingBoxMax = static_cast<float>(map.width());
    if (boundingBoxMax < static_cast<float>(map.height()))
        boundingBoxMax = static_cast<float>(map.height());
    if (boundingBoxMax < static_cast<float>(map.deep()))
        boundingBoxMax = static_cast<float>(map.deep());

    constexpr float offsetJumpingBall = 1.f; //size of ball + jump height
    float halfBoundingBoxSize = std::move(boundingBoxMax);
    halfBoundingBoxSize = halfBoundingBoxSize/2.f + offsetJumpingBall;

    _uniformMatrix4["VP"] =
    glm::mat4(glm::ortho(-halfBoundingBoxSize,
                         halfBoundingBoxSize,
                        -halfBoundingBoxSize, halfBoundingBoxSize,
                        _camera.zNear, _camera.zFar) *
              glm::lookAt ( _star.centralPosition(), center,
                            glm::vec3(0.f,1.f,0.f) ));
    _uniformMatrix4["VPStar"] = _uniformMatrix4["VP"];
    sp.bindUniform ("VP", _uniformMatrix4.at("VP"));
}

const std::string Rendering::vsshaderMap = "shaders/phongVs.vs";
const std::string Rendering::fsshaderMap = "shaders/phongFs.fs";

const std::string Rendering::vsshaderStar = "shaders/starVs.vs";
const std::string Rendering::fsshaderStar = "shaders/starFs.fs";

const std::string Rendering::vsshaderFBO = "shaders/basicFboVs.vs";
const std::string Rendering::fsshaderFBO = "shaders/basicFboFs.fs";

const std::string Rendering::vsshaderBlur = "shaders/basicFboVs.vs";
const std::string Rendering::fsshaderBlur = "shaders/blurFs.fs";

const std::string Rendering::vsshaderBrightPassFilter = "shaders/basicFboVs.vs";
const std::string Rendering::fsshaderBrightPassFilter =
                                                  "shaders/brightPassFilter.fs";

const std::string Rendering::vsshaderBloom = "shaders/basicFboVs.vs";
const std::string Rendering::fsshaderBloom = "shaders/bloomFs.fs";
const std::string Rendering::vsshaderDepth = "shaders/depthVs.vs";
const std::string Rendering::fsshaderDepth = "shaders/depthFs.fs";

const std::vector<float> Rendering::gaussComputedValues =
  Utility::genGaussBuffer(Rendering::blurPatchSize, Rendering::blurSigma);

const glm::vec3 Rendering::backgroundColor {0.f,0.f,.1f};
