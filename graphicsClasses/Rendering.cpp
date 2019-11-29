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

    

    _spMap.use();

    glEnable(GL_DEPTH_TEST);  

}


Rendering::verticesAttributeData<GLfloat> Rendering::mapVertices() {
    Rendering::verticesAttributeData<GLfloat> data;
    return data;
}

void Rendering::render() {

    renderCamera();
    //_map.render();

    GLuint modelWorldID = glGetUniformLocation(_spMap.getHandle(), "MW");
    glm::mat4 matModelWorld = _ball.world() * _ball.local();
    glUniformMatrix4fv(modelWorldID, 1, GL_FALSE, &matModelWorld[0][0]);

    _ball.render();
    Utility::printMatrix(matModelWorld);

    matModelWorld = glm::mat4(1.f);
    glUniformMatrix4fv(modelWorldID, 1, GL_FALSE, &matModelWorld[0][0]);
    _map.render();
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

void Rendering::renderBall() {

    std::array<std::vector<GLfloat>,3 >sphere = Utility::getLocalSphere();
    for (unsigned int i = 0 ; i < sphere.at(0).size(); i += 3 )
    {
        sphere.at(0).at(i)   +=  static_cast<GLfloat>   (3) ;
        sphere.at(0).at(i+1) +=  static_cast<GLfloat>   (3);
        sphere.at(0).at(i+2) +=  static_cast<GLfloat>   (6) ;
    }

    _vData[Rendering::Attribute::Positions].insert(
    _vData[Rendering::Attribute::Positions].end(),
            sphere.at(0).begin(),sphere.at(0).end()
    );

    _vData[Rendering::Attribute::Colors].insert(
    _vData[Rendering::Attribute::Colors].end(),
            sphere.at(1).begin(), sphere.at(1).end()
    );
}

void Rendering::renderCamera() {
  _uniformMatrix4["VP"] =  glm::mat4(
  glm::perspective(glm::radians(70.f), 4.f/3.f, _camera._zNear, _camera._zFar)
  * glm::lookAt(glm::vec3(7,5,8), glm::vec3(0,0,0), glm::vec3(0,1,0)));
  //* glm::lookAt(glm::vec3(5,2,2), glm::vec3(0,0,0), glm::vec3(0,1,0)));
 
GLuint MatrixID = glGetUniformLocation(_spMap.getHandle(), "VP");

glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &_uniformMatrix4.at("VP")[0][0]);

}




Rendering::~Rendering() {

}

