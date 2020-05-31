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
    _spToneMapping
          ( Shader (GL_VERTEX_SHADER,   vsshaderToneMapping),
            Shader (GL_FRAGMENT_SHADER, fsshaderToneMapping)),
    _spBrightPassFilter
          ( Shader (GL_VERTEX_SHADER,   vsshaderBrightPassFilter),
            Shader (GL_FRAGMENT_SHADER, fsshaderBrightPassFilter)),
    _spBloom
          ( Shader (GL_VERTEX_SHADER,   vsshaderBloom),
            Shader (GL_FRAGMENT_SHADER, fsshaderBloom)),
    _spDepth
          ( Shader (GL_VERTEX_SHADER,   vsshaderDepth),
            Shader (GL_FRAGMENT_SHADER, fsshaderDepth)),
    _frameBufferDepth(FrameBuffer::TextureCaterory::HDR),
    _frameBufferScene(FrameBuffer::TextureCaterory::HDR),
    _frameBufferToneMapping(FrameBuffer::TextureCaterory::SDR,false),
    _frameBufferHalfBlur(FrameBuffer::TextureCaterory::SDR,false,
        heightBloomTexture / static_cast<float>(Utility::windowResolutionY)),
    _frameBufferCompleteBlur(FrameBuffer::TextureCaterory::SDR,false,
        heightBloomTexture / static_cast<float>(Utility::windowResolutionY)),
    _frameBufferBrightPassFilter(FrameBuffer::TextureCaterory::SDR,false,
        heightBloomTexture / static_cast<float>(Utility::windowResolutionY)),
    _frameBufferBloom(FrameBuffer::TextureCaterory::SDR,false)
{
}

void Rendering::phongEffect( const FrameBuffer& referenceFBO ) {

    glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
    _frameBufferScene.bindFrameBuffer();
    _spMap.use();
    _spMap.bindUniformTexture("depthTexture", 0,
                              referenceFBO.getRenderTexture());

    //Ball and Map
    bindCamera (_spMap);

    _light.positionLight(_star.centralPosition());
                                        
    _light.bind("light",_spMap);

    _meshBall.render(_spMap);

    //Map
    _meshMap.render(_spMap);

    //Star
    _spStar.use();

    _spStar.bindUniform("radiusInside",  _star.radiusInside());
    _spStar.bindUniform("radiusOutside", _star.radiusOutside());
    _spStar.bindUniform("colorInside",   _star.colorInside());
    _spStar.bindUniform("colorOutside",  _star.colorOutside());

    bindCamera(_spStar);

    _meshStar.render(_spStar);

}

void Rendering::blurEffect( const FrameBuffer& referenceFBO ) {

    _spBlur.use();
    _frameBufferHalfBlur.bindFrameBuffer();

    _spBlur.bindUniformTexture("frameTexture", 0,
                               referenceFBO.getRenderTexture());
    _spBlur.bindUniform("patchSize", static_cast<int>(blurPatchSize));
    _spBlur.bindUniform("gaussWeights", gaussComputedValues);

    _spBlur.bindUniform("firstPass", true);
    _meshQuadFrame.render(_spBlur);
    _frameBufferCompleteBlur.bindFrameBuffer();
    _spBlur.bindUniformTexture("frameTexture", 0,
                               _frameBufferHalfBlur.getRenderTexture());

    _spBlur.bindUniform("firstPass", false);
    _meshQuadFrame.render(_spBlur);
}

void Rendering::toneMappingEffect( const FrameBuffer& referenceFBO) {

    std::pair<float,float> averageLuminanceAndMax {0.f,0.f};
    //referenceFBO.computeLogAverageLuminanceAndMax();
    

    _spToneMapping.use();
    _frameBufferToneMapping.bindFrameBuffer();

    _spToneMapping.bindUniformTexture("frameTexture", 0,
                                      referenceFBO.getRenderTexture());
    _spToneMapping.bindUniform ( "averageLuminance", 
                  //averageLuminanceAndMax.first,
                  //referenceFBO.computeLogAverageLuminance(),
                    1.8f);
    _spToneMapping.bindUniform ( "whiteLuminance", 
                    averageLuminanceAndMax.second);
                  //referenceFBO.computeLogAverageLuminance(),
                  //1.8f,
    
    _meshQuadFrame.render(_spToneMapping);
}

void Rendering::brightPassEffect( const FrameBuffer& referenceFBO) {
    _spBrightPassFilter.use();
    _frameBufferBrightPassFilter.bindFrameBuffer();

    _spBrightPassFilter.bindUniformTexture("frameTexture", 0,
                                           referenceFBO.getRenderTexture());
    _spBrightPassFilter.bindUniform ("threshold",  5.f);
    _meshQuadFrame.render(_spBrightPassFilter);
}

void Rendering::bloomEffect(  const FrameBuffer& fboScene, 
                              const FrameBuffer& fboLight) {
    _spBloom.use();
    _frameBufferBloom.bindFrameBuffer();

    _spBloom.bindUniformTexture("frameToneMappingTexture", 0,
                                fboScene.getRenderTexture());

    _spBloom.bindUniformTexture("frameBrightPassFilterTexture", 1,
                                fboLight.getRenderTexture());
    _meshQuadFrame.render(_spBloom);
}

void Rendering::depthFromStar() {

    glClearColor(1.0f, 1.0f, 1.f, 0.0f);
    _frameBufferDepth.bindFrameBuffer();
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
    glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    _meshMap.update();
    _meshBall.update();
    _meshStar.update();

    depthFromStar();

    phongEffect(_frameBufferDepth);

    toneMappingEffect(_frameBufferScene);
     
    brightPassEffect(_frameBufferScene);
    blurEffect(_frameBufferBrightPassFilter);

    bloomEffect(_frameBufferToneMapping,_frameBufferCompleteBlur);

    _spFbo.use();
    FrameBuffer::bindDefaultFrameBuffer();
    _spFbo.bindUniformTexture("frameTexture", 0,
                              _frameBufferBloom.getRenderTexture());
    _meshQuadFrame.render(_spFbo);

}

void Rendering::bindCamera(const ShaderProgram& sp) {

  _uniformBool["displayBehind"] = _camera.displayBehind();

  _uniformMatrix4["VP"] =  
          glm::mat4( 
                    glm::perspective( glm::radians(70.f), 
                    static_cast<float>(Utility::windowResolutionX)/
                    static_cast<float>(Utility::windowResolutionY),
                    _camera.zNear, _camera.zFar) *
                    glm::lookAt ( _camera.pos(), _camera.dir(), _camera.up())
                    ); 

  const JumperBallTypes::vec3f& positionBall = _ball.get3DPosition(); 

  _uniformVec3["positionBall"]    = glm::vec3( positionBall.x,
                                               positionBall.y,
                                               positionBall.z);

  _uniformVec3["lookDirection"]   = 
      glm::normalize( glm::cross  ( _camera.up() , 
                                    glm::cross( _camera.dir() - _camera.pos(),
                                                _camera.up()))
                                  );

  _uniformVec3["positionCamera"]  = _camera.pos();
  _uniformFloat["distanceBehind"] = _ball.distanceBehindBall();
  sp.bindUniform ("VP",             _uniformMatrix4.at("VP"));
  sp.bindUniform ("VPStar",         _uniformMatrix4.at("VPStar"));
  sp.bindUniform ("positionBall",   _uniformVec3.at("positionBall"));
  sp.bindUniform ("lookDirection",  _uniformVec3.at("lookDirection"));
  sp.bindUniform ("distanceBehind", _uniformFloat.at("distanceBehind"));
  sp.bindUniform ("positionCamera", _uniformVec3.at("positionCamera"));
  sp.bindUniform ("displayBehind",  _uniformBool.at("displayBehind"));
}

void Rendering::bindStarView(const ShaderProgram& sp) {
     
    const Map& map = _meshMap.base();

    const glm::vec3 center{ map.boundingBoxXMax()/2.f,
                            map.boundingBoxYMax()/2.f,
                            map.boundingBoxZMax()/2.f };
    
    float boundingBoxMax = static_cast<float>(map.boundingBoxXMax());
    if (boundingBoxMax < static_cast<float>(map.boundingBoxYMax()))
        boundingBoxMax = static_cast<float>(map.boundingBoxYMax());
    if (boundingBoxMax < static_cast<float>(map.boundingBoxZMax())) 
        boundingBoxMax = static_cast<float>(map.boundingBoxZMax());
    
    constexpr float offsetJumpingBall = 1.f; //size of ball + jump height
    float halfBoundingBoxSize = std::move(boundingBoxMax);
    halfBoundingBoxSize = halfBoundingBoxSize/2.f + offsetJumpingBall;

    _uniformMatrix4["VP"] =
    glm::mat4(glm::ortho(-halfBoundingBoxSize*
                         Utility::windowResolutionX/Utility::windowResolutionY,
                         halfBoundingBoxSize*
                         Utility::windowResolutionX/Utility::windowResolutionY,
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

const std::string Rendering::vsshaderToneMapping = "shaders/basicFboVs.vs";
const std::string Rendering::fsshaderToneMapping = "shaders/toneMappingFs.fs";

const std::string Rendering::vsshaderBrightPassFilter = "shaders/basicFboVs.vs";
const std::string Rendering::fsshaderBrightPassFilter = 
                                                  "shaders/brightPassFilter.fs";

const std::string Rendering::vsshaderBloom = "shaders/basicFboVs.vs";
const std::string Rendering::fsshaderBloom = "shaders/bloomFs.fs";
const std::string Rendering::vsshaderDepth = "shaders/depthVs.vs";
const std::string Rendering::fsshaderDepth = "shaders/depthFs.fs";

const std::vector<float> Rendering::gaussComputedValues = 
  Utility::genGaussBuffer(Rendering::blurPatchSize, Rendering::blurSigma);

