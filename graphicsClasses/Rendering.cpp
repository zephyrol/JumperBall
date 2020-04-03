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
    _frameBufferScene(true),
    _frameBufferToneMapping(false,false),
    _frameBufferHalfBlur(false,false,scaleBloomTexture),
    _frameBufferCompleteBlur(false,false,scaleBloomTexture),
    _frameBufferBrightPassFilter(false,false,scaleBloomTexture),
    _frameBufferBloom(false,false)
{
}


void Rendering::blurEffect( const FrameBuffer& referenceFBO ) {

    _spBlur.use();
    _frameBufferHalfBlur.bindFrameBuffer();

    referenceFBO.bindRenderTexture();
    _spBlur.bindUniformTexture("frameTexture", 0);
    _spBlur.bindUniform("patchSize", static_cast<int>(blurPatchSize));
    _spBlur.bindUniform("gaussWeights", gaussComputedValues);

    _spBlur.bindUniform("firstPass", true);
    _meshQuadFrame.render(_spBlur);

    _frameBufferCompleteBlur.bindFrameBuffer();
    _frameBufferHalfBlur.bindRenderTexture();

    _spBlur.bindUniform("firstPass", false);
    _meshQuadFrame.render(_spBlur);
}

void Rendering::toneMappingEffect( const FrameBuffer& referenceFBO) {

    std::pair<float,float> averageLuminanceAndMax {0.f,0.f};
    //referenceFBO.computeLogAverageLuminanceAndMax();
    

    _spToneMapping.use();
    _frameBufferToneMapping.bindFrameBuffer();

    referenceFBO.bindRenderTexture();
    _spToneMapping.bindUniformTexture("frameTexture", 0);
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

    referenceFBO.bindRenderTexture();
    _spBrightPassFilter.bindUniformTexture("frameTexture", 0);
    _spBrightPassFilter.bindUniform ("threshold",  5.f);
    _meshQuadFrame.render(_spBrightPassFilter);
}

void Rendering::bloomEffect(  const FrameBuffer& fboScene, 
                              const FrameBuffer& fboLight) {
    _spBloom.use();
    _frameBufferBloom.bindFrameBuffer();

    fboScene.bindRenderTexture(0);
    _spBloom.bindUniformTexture("frameToneMappingTexture", 0);

    fboLight.bindRenderTexture(1);
    _spBloom.bindUniformTexture("frameBrightPassFilterTexture", 1);
    _meshQuadFrame.render(_spBloom);
}


void Rendering::render() {

    _frameBufferScene.bindFrameBuffer();
    //FrameBuffer::bindDefaultFrameBuffer();
    
    //Ball and Map
    _spMap.use();
    renderCamera(_spMap);

    _light.positionLight(_star.centralPosition());
                                        
    _light.bind("light",_spMap);

    _meshBall.update();
    _meshBall.render(_spMap);

    //Map
    _meshMap.update();
    _meshMap.render(_spMap);

    //Star
    _spStar.use();

    _spStar.bindUniform("radiusInside",  _star.radiusInside());
    _spStar.bindUniform("radiusOutside", _star.radiusOutside());
    _spStar.bindUniform("colorInside",   _star.colorInside());
    _spStar.bindUniform("colorOutside",  _star.colorOutside());

    renderCamera(_spStar);

    _meshStar.update();
    _meshStar.render(_spStar);

    toneMappingEffect(_frameBufferScene);
     
    brightPassEffect(_frameBufferScene);
    blurEffect(_frameBufferBrightPassFilter);

    bloomEffect(_frameBufferToneMapping,_frameBufferCompleteBlur);

    _spFbo.use();
    FrameBuffer::bindDefaultFrameBuffer();
    _frameBufferBloom.bindRenderTexture();
    _spFbo.bindUniformTexture("frameTexture", 0);
    _meshQuadFrame.render(_spFbo);

         

}


void Rendering::renderCamera(const ShaderProgram& sp) {

  _uniformBool["displayBehind"] = _camera.displayBehind();

  _uniformMatrix4["VP"] =  
          glm::mat4( 
                    glm::perspective( glm::radians(70.f), 
                    static_cast<float>(RESOLUTION_X)/
                    static_cast<float>(RESOLUTION_Y), 
                    _camera._zNear, _camera._zFar) *
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
  sp.bindUniform ("VP",              _uniformMatrix4.at("VP"));
  sp.bindUniform ("positionBall",   _uniformVec3.at("positionBall"));
  sp.bindUniform ("lookDirection",  _uniformVec3.at("lookDirection"));
  sp.bindUniform ("distanceBehind", _uniformFloat.at("distanceBehind"));
  sp.bindUniform ("positionCamera", _uniformVec3.at("positionCamera"));
  sp.bindUniform ("displayBehind",  _uniformBool.at("displayBehind"));
}



Rendering::~Rendering() {

}


const std::string Rendering::vsshaderMap = "graphicsClasses/shaders/phongVs.vs";
const std::string Rendering::fsshaderMap = "graphicsClasses/shaders/phongFs.fs";

const std::string Rendering::vsshaderStar = "graphicsClasses/shaders/starVs.vs";
const std::string Rendering::fsshaderStar = "graphicsClasses/shaders/starFs.fs";

const std::string Rendering::vsshaderFBO = 
                                        "graphicsClasses/shaders/basicFboVs.vs";
const std::string Rendering::fsshaderFBO = 
                                        "graphicsClasses/shaders/basicFboFs.fs";

const std::string Rendering::vsshaderBlur = 
                                        "graphicsClasses/shaders/basicFboVs.vs";
const std::string Rendering::fsshaderBlur = "graphicsClasses/shaders/blurFs.fs";

const std::string Rendering::vsshaderToneMapping = 
                                        "graphicsClasses/shaders/basicFboVs.vs";
const std::string Rendering::fsshaderToneMapping = 
                                     "graphicsClasses/shaders/toneMappingFs.fs";

const std::string Rendering::vsshaderBrightPassFilter = 
                                        "graphicsClasses/shaders/basicFboVs.vs";
const std::string Rendering::fsshaderBrightPassFilter = 
                                  "graphicsClasses/shaders/brightPassFilter.fs";

const std::string Rendering::vsshaderBloom = 
                                        "graphicsClasses/shaders/basicFboVs.vs";
const std::string Rendering::fsshaderBloom = 
                                           "graphicsClasses/shaders/bloomFs.fs";

const std::vector<float> Rendering::gaussComputedValues = 
  Utility::genGaussBuffer(Rendering::blurPatchSize, Rendering::blurSigma);

