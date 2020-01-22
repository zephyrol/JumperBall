/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Rendering.cpp
 * Author: seb
 * 
 * Created on 6 novembre 2019, 20:38
 */

#include "Rendering.h"

const std::string Rendering::vsshaderMap = "graphicsClasses/shaders/phongVs.vs";
const std::string Rendering::fsshaderMap = "graphicsClasses/shaders/phongFs.fs";

const std::string Rendering::vsshaderStar = "graphicsClasses/shaders/starVs.vs";
const std::string Rendering::fsshaderStar = "graphicsClasses/shaders/starFs.fs";

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
    _map(map),
    _ball(ball),
    _ballAnimation(ball),
    _star(star),
    _light(),
    _camera(camera),
    _spMap( Shader (GL_VERTEX_SHADER, vsshaderMap ),
            Shader (GL_FRAGMENT_SHADER, fsshaderMap )),
    _spStar(Shader (GL_VERTEX_SHADER, vsshaderStar ),
            Shader (GL_FRAGMENT_SHADER, fsshaderStar ))
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


void Rendering::render() {
    
    //Ball and Map
    _spMap.use();
    renderCamera(_spMap);

    _light.positionLight(_star.centralPosition());
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
    bindUniform ("M",  glm::mat4(1.f), _spMap);
    bindUniform ("SR", glm::mat4(1.f), _spMap);

    bindUniform ("W",  glm::mat4(1.f), _spMap);

    
    
    _meshMap.draw();


    //Star
    _spStar.use();

    bindUniform ("MW",            _star.transform(),      _spStar);
    bindUniform ("radiusInside",  _star.radiusInside(),   _spStar);
    bindUniform ("radiusOutside", _star.radiusOutside(),  _spStar);
    bindUniform ("colorInside",   _star.colorInside(),    _spStar);
    bindUniform ("colorOutside",  _star.colorOutside(),   _spStar);

    renderCamera(_spStar);
    _star.draw();
     

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

