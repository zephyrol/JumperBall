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
    _idVertexArray(),
    _idVertexBuffer(),
    _vData(),
    _meshMap(map),
    _meshBall(ball),
    _map(map),
    _ball(ball),
    _ballPosition(ball.get3DPosition()),
    _camera(camera),
    _spMap( Shader (GL_VERTEX_SHADER, vsshaderMap ),
            Shader (GL_FRAGMENT_SHADER, fsshaderMap ))
{


    _spMap.use();

    glEnable(GL_DEPTH_TEST);  

}


Rendering::verticesAttributeData<GLfloat> Rendering::mapVertices() {
    Rendering::verticesAttributeData<GLfloat> data;
    return data;
}

void Rendering::render() {
    
    _ballPosition = _ball.get3DPosition();
    renderCamera();

    GLuint modelWorldID = glGetUniformLocation(_spMap.getHandle(), "MW");
    _meshBall.updateMatrices(_ball,_ballPosition);
    glm::mat4 matModelWorld = _meshBall.world() * _meshBall.local();
    glUniformMatrix4fv(modelWorldID, 1, GL_FALSE, &matModelWorld[0][0]);

    //Utility::printMatrix(matModelWorld);

    _meshBall.render();

    matModelWorld = glm::mat4(1.f);
    glUniformMatrix4fv(modelWorldID, 1, GL_FALSE, &matModelWorld[0][0]);
    _meshMap.render();


}


void Rendering::renderCamera() {


  _camera.follow(_ball,_ballPosition);

  const std::array<float,3> position  = _camera.pos();
  const std::array<float,3> direction = _camera.dir();
  const std::array<float,3> up        = _camera.up();

  _uniformMatrix4["VP"] =  glm::mat4(
  glm::perspective(glm::radians(70.f), 4.f/3.f, _camera._zNear, _camera._zFar)
  * glm::lookAt(glm::vec3(position.at(0), position.at(1), position.at(2)), 
                glm::vec3(direction.at(0),direction.at(1),direction.at(2)), 
                glm::vec3(up.at(0),       up.at(1),       up.at(2))));

  _uniformVec3["positionBall"] = glm::vec3( _ballPosition.at(0),
                                            _ballPosition.at(1),
                                            _ballPosition.at(2));

  const std::array<float,3> lookTowardsDir = _ball.lookTowardsThroughVector();
  _uniformVec3["lookDirection"] = glm::vec3(lookTowardsDir.at(0),
                                            lookTowardsDir.at(1),
                                            lookTowardsDir.at(2));


 
  const GLuint MatrixID = glGetUniformLocation(_spMap.getHandle(), "VP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &_uniformMatrix4.at("VP")[0][0]);
  
  const GLuint posBallID = glGetUniformLocation(_spMap.getHandle(), 
                                                "positionBall");
  glUniform3fv(posBallID, 1, &_uniformVec3.at("positionBall")[0]);

  const GLuint lookDirectionID = glGetUniformLocation(_spMap.getHandle(),
                                                      "lookDirection");
  glUniform3fv(lookDirectionID, 1, &_uniformVec3.at("lookDirection")[0]);

}




Rendering::~Rendering() {

}

