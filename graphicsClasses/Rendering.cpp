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

const std::map<Rendering::Attribute,unsigned int> Rendering::nbComponents {
    {Rendering::Attribute::Positions,  3},
    {Rendering::Attribute::Normals,    3},
    {Rendering::Attribute::Colors,     3},
    {Rendering::Attribute::UVCoords,   2}
};

const std::string Rendering::vsshaderMap = "graphicsClasses/shaders/mapVs.vs";
const std::string Rendering::fsshaderMap = "graphicsClasses/shaders/mapFs.fs";


Rendering::Rendering(const Map& map, const Ball& ball, const Camera& camera):
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
    _camera(camera),
    _spMap( Shader (GL_VERTEX_SHADER, vsshaderMap ),
            Shader (GL_FRAGMENT_SHADER, fsshaderMap )),
    _idVertexArray(),
    _idVertexBuffer(),
    _vData()
{
    _spMap.use();
    glEnable(GL_DEPTH_TEST);  
}


Rendering::verticesAttributeData<GLfloat> Rendering::mapVertices() {
    Rendering::verticesAttributeData<GLfloat> data;
    return data;
}

void Rendering::render() {
    
    renderCamera();

    GLuint modelWorldID = glGetUniformLocation(_spMap.getHandle(), "MW");
    _meshBall.updateMatrices(_ball);
    glm::mat4 matModelWorld = _meshBall.world() * _meshBall.local();
    glUniformMatrix4fv(modelWorldID, 1, GL_FALSE, &matModelWorld[0][0]);

    //Utility::printMatrix(matModelWorld);

    _meshBall.render();

    matModelWorld = glm::mat4(1.f);
    glUniformMatrix4fv(modelWorldID, 1, GL_FALSE, &matModelWorld[0][0]);
    _meshMap.render();


}


void Rendering::renderCamera() {


  _uniformMatrix4["VP"] =  glm::mat4(
  glm::perspective(glm::radians(70.f), 4.f/3.f, _camera._zNear, _camera._zFar)
  * glm::lookAt ( _camera.pos(), _camera.dir(), _camera.up())); 

  const std::array<float,3> positionBall = _ball.get3DPosition();
  _uniformVec3["positionBall"] = glm::vec3( positionBall.at(0),
                                            positionBall.at(1),
                                            positionBall.at(2));

  _uniformVec3["lookDirection"] = 
      glm::normalize( glm::cross  ( _camera.up() , 
                                    glm::cross( _camera.dir() - _camera.pos(),
                                                _camera.up()))
                                  );

  _uniformFloat["distanceBehind"] = _ball.distanceBehindBall();
 
  const GLuint MatrixID         = glGetUniformLocation(_spMap.getHandle(),"VP");
  glUniformMatrix4fv(MatrixID,  1, GL_FALSE, &_uniformMatrix4.at("VP")[0][0]);
  
  const GLuint posBallID        = glGetUniformLocation(_spMap.getHandle(), 
                                                        "positionBall");
  glUniform3fv(posBallID,       1, &_uniformVec3.at("positionBall")[0]);

  const GLuint lookDirectionID  = glGetUniformLocation(_spMap.getHandle(),
                                                        "lookDirection");
  glUniform3fv(lookDirectionID, 1, &_uniformVec3.at("lookDirection")[0]);

  const GLuint distanceBehindID = glGetUniformLocation(_spMap.getHandle(),
                                                        "distanceBehind");
  glUniform1fv(distanceBehindID,1, &_uniformFloat.at("distanceBehind"));
}

Rendering::~Rendering() {

}

