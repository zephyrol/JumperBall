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

const std::string Rendering::vsshaderMap  = "graphicsClasses/shaders/mapVs.vs";
const std::string Rendering::fsshaderMap  = "graphicsClasses/shaders/mapFs.fs";

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
    _star(star),
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

void Rendering::bindUniformBlock(const std::string& name, const bool& value, const ShaderProgram& sp) {
     
    // Index of block
    const GLuint blockIndex = glGetUniformBlockIndex( sp.getHandle()
                                                      ,name.c_str());
    (void) value;
    // Allocate space for the buffer to contain the data of the block
    GLint blockSize;
    glGetActiveUniformBlockiv ( sp.getHandle(), blockIndex, 
                                GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    std::vector<GLubyte> dataBuffer (blockSize);

}


void Rendering::render() {
    
    //Ball and Map
    _spMap.use();
    renderCamera(_spMap);

    //Ball
    _meshBall.updateMatrices(_ball);

    bindUniform ("MW", _meshBall.world() * _meshBall.local(), _spStar);

    _meshBall.draw();

    //Map
    bindUniform ("MW", glm::mat4(1.f), _spStar);
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

  const std::string nameVP = "VP";
  const std::string namePositionBall = "positionBall";
  const std::string nameDistanceBehind = "distanceBehind";
  const std::string nameLookDirection = "lookDirection";

  _uniformMatrix4[nameVP] =  glm::mat4(
  glm::perspective(glm::radians(70.f), 4.f/3.f, _camera._zNear, _camera._zFar)
  * glm::lookAt ( _camera.pos(), _camera.dir(), _camera.up())); 

  const std::array<float,3> positionBall = _ball.get3DPosition();
  _uniformVec3[namePositionBall] = glm::vec3( positionBall.at(0),
                                            positionBall.at(1),
                                            positionBall.at(2));

  _uniformVec3[nameLookDirection] = 
      glm::normalize( glm::cross  ( _camera.up() , 
                                    glm::cross( _camera.dir() - _camera.pos(),
                                                _camera.up()))
                                  );

  _uniformFloat[nameDistanceBehind] = _ball.distanceBehindBall();

  bindUniform (nameVP,              _uniformMatrix4.at(nameVP),           sp);
  bindUniform (namePositionBall,    _uniformVec3.at(namePositionBall),    sp);
  bindUniform (nameLookDirection,   _uniformVec3.at(nameLookDirection),   sp);
  bindUniform (nameDistanceBehind,  _uniformFloat.at(nameDistanceBehind), sp);
}



Rendering::~Rendering() {

}

