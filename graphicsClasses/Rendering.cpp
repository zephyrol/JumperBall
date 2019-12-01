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
    _camera(camera),
    _spMap( Shader (GL_VERTEX_SHADER, vsshaderMap ),
            Shader (GL_FRAGMENT_SHADER, fsshaderMap ))
{

    /*glGenVertexArrays(1, &_idVertexArray);
    glBindVertexArray(_idVertexArray);

    glGenBuffers(2, _idVertexBuffer.data());*/

    /*renderMap();
    renderBall();*/
    /*_map.render();
    renderCamera(); */


    /*glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(0));
    glBufferData(GL_ARRAY_BUFFER, _vData.at(Rendering::Attribute::Positions)
            .size() * sizeof(GLfloat), 
            _vData.at(Rendering::Attribute::Positions).data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(1));
    glBufferData(GL_ARRAY_BUFFER, _vData.at(Rendering::Attribute::Colors)
            .size() * sizeof(GLfloat), 
            _vData.at(Rendering::Attribute::Colors).data(), GL_STATIC_DRAW);*/

    
    _camera.follow(ball);
    _spMap.use();

    glEnable(GL_DEPTH_TEST);  

}


Rendering::verticesAttributeData<GLfloat> Rendering::mapVertices() {
    Rendering::verticesAttributeData<GLfloat> data;
    return data;
}

void Rendering::render() {
    
    _camera.follow(_ball);
    renderCamera();
    //_map.render();

    GLuint modelWorldID = glGetUniformLocation(_spMap.getHandle(), "MW");
    _meshBall.updateMatrices(_ball);
    glm::mat4 matModelWorld = _meshBall.world() * _meshBall.local();
    glUniformMatrix4fv(modelWorldID, 1, GL_FALSE, &matModelWorld[0][0]);

    //Utility::printMatrix(matModelWorld);

    _meshBall.render();

    matModelWorld = glm::mat4(1.f);
    glUniformMatrix4fv(modelWorldID, 1, GL_FALSE, &matModelWorld[0][0]);
    _meshMap.render();


    /*glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(0));
    
    glVertexAttribPointer ( 
            0,
            3, // 3 GL_FLOAT per vertex
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
            );
    
    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(1));
    glVertexAttribPointer ( 
            1,
            3, // 3 GL_FLOAT per vertex
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
            );
    glDrawArrays(GL_TRIANGLES,0,_vData.at(Rendering::Attribute::Positions)
            .size());*/
}

/*void Rendering::renderMap() {
    for (unsigned int x = 0; x < _map.boundingBoxXMax() ; ++x ) {
        for (unsigned int y = 0; y < _map.boundingBoxYMax() ; ++y ) {
            for (unsigned int z = 0; z < _map.boundingBoxZMax() ; ++z ) {
                auto block = _map.map3DData(x,y,z);
                if (block) {
                    std::vector<GLfloat> pCube=Utility::getPositionsLocalCube();
                    for (unsigned int i = 0 ; i < pCube.size(); i += 3 )
                    {
                        pCube.at(i)   +=  static_cast<GLfloat> (x) ;
                        pCube.at(i+1) +=  static_cast<GLfloat> (y) ;
                        pCube.at(i+2) +=  static_cast<GLfloat> (z) ;
                    }
                    _vData[Rendering::Attribute::Positions].insert(
                    _vData[Rendering::Attribute::Positions].end(),
                            pCube.begin(), pCube.end()
                    );

                    std::vector<GLfloat> cCube=Utility::getColorsLocalCube();
                    _vData[Rendering::Attribute::Colors].insert(
                    _vData[Rendering::Attribute::Colors].end(),
                            cCube.begin(), cCube.end()
                    );

                }
            }
        }
    }
}*/

void Rendering::renderCamera() {
    const std::array<float,3> position  = _camera.pos();
    const std::array<float,3> direction = _camera.dir();
    const std::array<float,3> up        = _camera.up();

  _uniformMatrix4["VP"] =  glm::mat4(
  glm::perspective(glm::radians(70.f), 4.f/3.f, _camera._zNear, _camera._zFar)
  * glm::lookAt(glm::vec3(position.at(0), position.at(1), position.at(2)), 
                glm::vec3(direction.at(0),direction.at(1),direction.at(2)), 
                glm::vec3(up.at(0),       up.at(1),       up.at(2))));

  std::array<float,3> posBall = _ball.get3DPos();
  _uniformVec3["positionBall"] = glm::vec3(posBall.at(0),
                                           posBall.at(1),
                                           posBall.at(2));

  std::array<float,3> lookTowardsDir = _ball.lookTowardsThroughVector();
  _uniformVec3["lookDirection"] = glm::vec3(lookTowardsDir.at(0),
                                           lookTowardsDir.at(1),
                                           lookTowardsDir.at(2));


 
  GLuint MatrixID = glGetUniformLocation(_spMap.getHandle(), "VP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &_uniformMatrix4.at("VP")[0][0]);
  
  GLuint posBallID = glGetUniformLocation(_spMap.getHandle(), "positionBall");
  glUniform3fv(posBallID, 1, &_uniformVec3.at("positionBall")[0]);

  GLuint lookDirectionID = glGetUniformLocation(_spMap.getHandle(),
                                                "lookDirection");
  glUniform3fv(lookDirectionID, 1, &_uniformVec3.at("lookDirection")[0]);

}




Rendering::~Rendering() {

}

