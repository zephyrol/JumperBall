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
    _meshMap(map),
    _meshBall(ball),
    _meshQuadFrame(),
    _map(map),
    _ball(ball),
    _ballAnimation(ball),
    _star(star),
    _light(),
    _camera(camera),
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
    _frameBufferToneMapping(false),
    _frameBufferHalfBlur(false),
    _frameBufferCompleteBlur(false),
    _frameBufferBrightPassFilter(false),
    _frameBufferBloom(false)
{
}

void Rendering::bindUniform(const std::string& name, 
                            const glm::mat4& value, 
                            const ShaderProgram& sp) {

    const GLuint uniformVariableID =
            glGetUniformLocation(sp.getHandle(),name.c_str());
    glUniformMatrix4fv( uniformVariableID, 1, GL_FALSE, &value[0][0]);
}

void Rendering::bindUniform(const std::string& name, 
                            const glm::vec4& value, 
                            const ShaderProgram& sp) {

    const GLuint uniformVariableID =
            glGetUniformLocation(sp.getHandle(),name.c_str());
    glUniform4fv( uniformVariableID, 1, &value[0]);
}

void Rendering::bindUniform(const std::string& name, 
                            const glm::vec3& value, 
                            const ShaderProgram& sp) {

    const GLuint uniformVariableID =
            glGetUniformLocation(sp.getHandle(),name.c_str());
    glUniform3fv( uniformVariableID, 1, &value[0]);
}

void Rendering::bindUniform(const std::string& name, 
                            const glm::vec2& value, 
                            const ShaderProgram& sp) {

    const GLuint uniformVariableID =
            glGetUniformLocation(sp.getHandle(),name.c_str());
    glUniform2fv( uniformVariableID, 1, &value[0]);
}

void Rendering::bindUniform(const std::string& name, 
                            const GLfloat& value, 
                            const ShaderProgram& sp) {

    const GLuint uniformVariableID =
            glGetUniformLocation(sp.getHandle(),name.c_str());
    glUniform1fv( uniformVariableID, 1, &value);
}

void Rendering::bindUniform(const std::string& name, 
                            const bool& value, 
                            const ShaderProgram& sp) {

    const GLuint uniformVariableID =
            glGetUniformLocation(sp.getHandle(),name.c_str());
    glUniform1i( uniformVariableID, value);
}


void Rendering::bindUniform(const std::string& name, 
                            const int& value, 
                            const ShaderProgram& sp) {

    const GLuint uniformVariableID =
            glGetUniformLocation(sp.getHandle(),name.c_str());
    glUniform1i( uniformVariableID, value);
}

void Rendering::bindUniform(const std::string& name,
                            const std::vector<int>& value,
                            const ShaderProgram& sp) {
    const GLuint uniformVariableID =
            glGetUniformLocation(sp.getHandle(),name.c_str());
    glUniform1iv( uniformVariableID, value.size(), value.data());
}

void Rendering::bindUniform(const std::string& name,
                            const std::vector<float>& value,
                            const ShaderProgram& sp) {
    const GLuint uniformVariableID =
            glGetUniformLocation(sp.getHandle(),name.c_str());
    glUniform1fv( uniformVariableID, value.size(), value.data());
}


void Rendering::bindUniformTexture( const std::string& name, 
                                    const int& value, 
                                    const ShaderProgram& sp) {
    const GLuint uniformVariableID =
            glGetUniformLocation(sp.getHandle(),name.c_str());
    glUniform1i( uniformVariableID, value);
}


void Rendering::renderMap() {

    bindUniform ("M",  glm::mat4(1.f), _spMap);
    bindUniform ("SR", glm::mat4(1.f), _spMap);
    unsigned int displayedCubes = 0;
    constexpr unsigned int numberFaces = 6;
    constexpr unsigned int numberVerticesPerFaces = 6;
    constexpr unsigned int numberVerticesPerBlock = numberFaces*
                                                      numberVerticesPerFaces;
    for (unsigned int i = 0; i < _map.boundingBoxXMax() ; ++i){
        for (unsigned int j = 0; j < _map.boundingBoxYMax() ; ++j){
            for (unsigned int k = 0; k < _map.boundingBoxZMax() ; ++k){
                auto block = _map.getBlock(i,j,k);
                if (block){
                    const std::array<float,9>& transform = 
                          block->localTransform();
                    glm::mat4 trans = glm::translate(glm::vec3( transform.at(0),
                                              transform.at(1),transform.at(2)));
                    bindUniform ("W",  trans, _spMap);
                    _meshMap.draw(false,displayedCubes++*numberVerticesPerBlock,
                                        numberVerticesPerBlock);

                }
            }
        }
    }
    _meshMap.draw(true,displayedCubes*numberVerticesPerBlock,0);
}

void Rendering::blurEffect( const FrameBuffer& referenceFBO ) {

    _spBlur.use();
    _frameBufferHalfBlur.bindFrameBuffer();

    referenceFBO.bindRenderTexture();
    bindUniformTexture("frameTexture", 0, _spBlur);
    bindUniform("patchSize", static_cast<int>(blurPatchSize), _spBlur);
    bindUniform("gaussWeights", gaussComputedValues, _spBlur);

    bindUniform("firstPass", true, _spBlur);
    _meshQuadFrame.draw();

    _frameBufferCompleteBlur.bindFrameBuffer();
    _frameBufferHalfBlur.bindRenderTexture();

    bindUniform("firstPass", false, _spBlur);
    _meshQuadFrame.draw();
}

void Rendering::toneMappingEffect( const FrameBuffer& referenceFBO) {

    std::pair<float,float> averageLuminanceAndMax {0.f,0.f};
    //referenceFBO.computeLogAverageLuminanceAndMax();
    

    _spToneMapping.use();
    _frameBufferToneMapping.bindFrameBuffer();

    referenceFBO.bindRenderTexture();
    bindUniformTexture("frameTexture", 0, _spToneMapping);
    bindUniform ( "averageLuminance", 
                  //averageLuminanceAndMax.first,
                  //referenceFBO.computeLogAverageLuminance(),
                  1.8f,
                  _spToneMapping);
    bindUniform ( "whiteLuminance", 
                  averageLuminanceAndMax.second,
                  //referenceFBO.computeLogAverageLuminance(),
                  //1.8f,
                  _spToneMapping);
    
    _meshQuadFrame.draw();
}

void Rendering::brightPassEffect( const FrameBuffer& referenceFBO) {
    _spBrightPassFilter.use();
    _frameBufferBrightPassFilter.bindFrameBuffer();

    referenceFBO.bindRenderTexture();
    bindUniformTexture("frameTexture", 0, _spBrightPassFilter);
    bindUniform ("threshold",  5.f,   _spBrightPassFilter);
    _meshQuadFrame.draw();
}

void Rendering::bloomEffect(  const FrameBuffer& fboScene, 
                              const FrameBuffer& fboLight) {
    _spBloom.use();
    _frameBufferBloom.bindFrameBuffer();

    fboScene.bindRenderTexture(0);
    bindUniformTexture("frameToneMappingTexture", 0, _spBloom);

    fboLight.bindRenderTexture(1);
    bindUniformTexture("frameBrightPassFilterTexture", 1, _spBloom);

    _meshQuadFrame.draw();
}


void Rendering::render() {

    _frameBufferScene.bindFrameBuffer();
    //FrameBuffer::bindDefaultFrameBuffer();
    
    //Ball and Map
    _spMap.use();
    renderCamera(_spMap);

    _light.positionLight(           _star.centralPosition());
    _light.ambiantLightIntensity(   glm::vec3(0.7f,0.7f,0.7f));
    _light.diffuseLightIntensity(   glm::vec3(0.25f,0.25f,0.25f));
    _light.specularLightIntensity(  glm::vec3(0.25f,0.25f,0.25f));
                                        
    _light.bind("light",_spMap);

    //Ball
    _meshBall.updateMatrices(_ball);
    _ballAnimation.updateTrans();

    bindUniform ("M",   _ballAnimation.model(),         _spMap);
    bindUniform ("SR",  _ballAnimation.scaleRotation(), _spMap);

    bindUniform ("W",   _meshBall.world(),              _spMap);

    _meshBall.draw();

    //Map
    renderMap();

    //Star
    _spStar.use();

    bindUniform ("MW",            _star.transform(),      _spStar);
    bindUniform ("radiusInside",  _star.radiusInside(),   _spStar);
    bindUniform ("radiusOutside", _star.radiusOutside(),  _spStar);
    bindUniform ("colorInside",   _star.colorInside(),    _spStar);
    bindUniform ("colorOutside",  _star.colorOutside(),   _spStar);

    renderCamera(_spStar);
    _star.draw();

    toneMappingEffect(_frameBufferScene);
     
    brightPassEffect(_frameBufferScene);
    blurEffect(_frameBufferBrightPassFilter);

    bloomEffect(_frameBufferToneMapping,_frameBufferCompleteBlur);

    /*FrameBuffer::bindDefaultFrameBuffer();
    _frameBufferCompleteBlur.bindRenderTexture();
    //_frameBufferScene.bindRenderTexture();
    bindUniformTexture("frameTexture", 0, _spBrightPassFilter);*/

    _spFbo.use();
    FrameBuffer::bindDefaultFrameBuffer();
    _frameBufferBloom.bindRenderTexture();
    bindUniformTexture("frameTexture", 0, _spFbo);
    _meshQuadFrame.draw();

         

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
  bindUniform ("VP",              _uniformMatrix4.at("VP"),           sp);
  bindUniform ("positionBall",    _uniformVec3.at("positionBall"),    sp);
  bindUniform ("lookDirection",   _uniformVec3.at("lookDirection"),   sp);
  bindUniform ("distanceBehind",  _uniformFloat.at("distanceBehind"), sp);
  bindUniform ("positionCamera",  _uniformVec3.at("positionCamera"),  sp);
  bindUniform ("displayBehind",   _uniformBool.at("displayBehind"),   sp);
}



Rendering::~Rendering() {

}

