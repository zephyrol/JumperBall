/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mesh.cpp
 * Author: seb
 * 
 * Created on 13 novembre 2019, 21:05
 */

#include "Mesh.h"
#include "glm/gtc/matrix_transform.hpp"


Mesh::Mesh(const Ball& ball):
      _positions(),
      _normals(),
      _colors(),
      _uvCoords(),
      _useIndexing(true),
      _indices(),
      _idElementBuffer(),
      _idVertexArray(),
      _idVertexBuffer(),
      _local(1.f),
      _world(1.f)
{
  
    glGenVertexArrays(1, &_idVertexArray);
    glBindVertexArray(_idVertexArray);

    glGenBuffers(2, _idVertexBuffer.data());



    const unsigned int  iParaCount  = 40;
    const unsigned int  iMeriCount  = 60;
    const float         r           = ball.getRadius();
    
    // Create a sphere ---------------------------------------------------------
    GLuint iVertexCount = iParaCount * iMeriCount;
    
    const float a1 = ( 180.0 / ( iParaCount - 1 ) ) * M_PI / 180.0;
    const float a2 = ( 360.0 / ( iMeriCount - 1 ) ) * M_PI / 180.0;
    
    // parallels ---------------------------------------------------------------
    for( unsigned int i = 0; i < iParaCount; ++i )
    {
        const float fAngle    = - static_cast<float>(M_PI) / 2.0f + a1 * ( i );
        const float z         = r * static_cast<float>(sin( fAngle ));
        const float fRadius   = r * static_cast<float>(cos( fAngle ));
        
        for( unsigned int j = 0; j < iMeriCount; ++j )
        {
            _positions.push_back(glm::vec3( 
                    fRadius * static_cast<float>(cos( a2 * j )), 
                    fRadius * static_cast<float>(sin( a2 * j )), 
                    z ));
            _uvCoords.push_back(glm::vec2( static_cast<float>(j)/ iMeriCount, 
                    static_cast<float>(iParaCount - i ) / iParaCount ));
        }
    }
    // compute normals ---------------------------------------------------------
    // on a 0 centered sphere : you just need to normalise the position!
    _normals.reserve(iVertexCount);
    
    for( unsigned int i = 0; i < iVertexCount; ++i )
    {
        _normals.push_back(glm::normalize( _positions[ i ] ));
        _colors.push_back(_normals[i]);
    }
    
    //GLuint iElementsCount = ( iMeriCount - 1 ) * ( iParaCount - 1 ) * 2 * 3; 
    // for quads split in 2
    
    for( unsigned int i = 0; i < ( iParaCount - 1 ); ++i )
    {
        for( unsigned int j = 0; j < ( iMeriCount - 1 ); ++j )
        {
            _indices.push_back(iMeriCount * i + j);
            _indices.push_back(iMeriCount * i + ( j + 1 ));
            _indices.push_back(iMeriCount * ( i + 1 ) + ( j + 1 ));
            _indices.push_back(iMeriCount * ( i + 1 ) + ( j + 1 ));
            _indices.push_back(iMeriCount * ( i + 1 ) + j);
            _indices.push_back(iMeriCount * i + j);
        }
    }
   
    std::array<float,3> positionBall = ball.get3DPosition();
    _world = glm::translate(_world, glm::vec3(positionBall.at(0),
                            positionBall.at(1) ,positionBall.at(2)));

    std::vector<GLfloat> positionsList ;
    for (glm::vec3 pos : _positions) {
      positionsList.push_back(pos.x) ;
      positionsList.push_back(pos.y) ;
      positionsList.push_back(pos.z) ;
    }

    std::vector<GLfloat> colorsList;
    for (glm::vec3 color : _colors) {
      colorsList.push_back(color.x) ;
      colorsList.push_back(color.y) ;
      colorsList.push_back(color.z) ;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(0));
    glBufferData(GL_ARRAY_BUFFER, positionsList.size() * sizeof(GLfloat), 
            positionsList.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(1));
    glBufferData(GL_ARRAY_BUFFER, colorsList.size() * sizeof(GLfloat), 
            colorsList.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &_idElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_idElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,_indices.size() * sizeof (GLushort),
            _indices.data(),GL_STATIC_DRAW);
}

Mesh::Mesh(const Map& map):
      _positions(),
      _normals(),
      _colors(),
      _uvCoords(),
      _useIndexing(false),
      _indices(),
      _idElementBuffer(),
      _idVertexArray(),
      _idVertexBuffer(),
      _local(1.f),
      _world(1.f)
{

    glGenVertexArrays(1, &_idVertexArray);
    glBindVertexArray(_idVertexArray);

    glGenBuffers(2, _idVertexBuffer.data());


    for (unsigned int x = 0; x < map.boundingBoxXMax() ; ++x ) {
        for (unsigned int y = 0; y < map.boundingBoxYMax() ; ++y ) {
            for (unsigned int z = 0; z < map.boundingBoxZMax() ; ++z ) {
                auto block = map.map3DData(x,y,z);
                if (block) {
                    std::vector<GLfloat> pCube=Utility::getPositionsLocalCube();

                    for (unsigned int i = 0 ; i < pCube.size(); i += 3 )
                    {
                        pCube.at(i)   +=  static_cast<GLfloat> (x) ;
                        pCube.at(i+1) +=  static_cast<GLfloat> (y) ;
                        pCube.at(i+2) +=  static_cast<GLfloat> (z) ;
                        _positions.push_back(glm::vec3(pCube.at(i),pCube.at(i+1)
                                          ,pCube.at(i+2)));
                    }

                    std::vector<GLfloat> cCube=Utility::getColorsLocalCube();
                    for (unsigned int i = 0 ; i < cCube.size(); i += 3 )
                    {
                        _colors.push_back(glm::vec3(cCube.at(i),cCube.at(i+1)
                                          ,cCube.at(i+2)));
                    }


                }
            }
        }
    }
    
    std::vector<GLfloat> positionsList ;
    for (glm::vec3 pos : _positions) {
      positionsList.push_back(pos.x) ;
      positionsList.push_back(pos.y) ;
      positionsList.push_back(pos.z) ;
    }

    std::vector<GLfloat> colorsList;
    for (glm::vec3 color : _colors) {
      colorsList.push_back(color.x) ;
      colorsList.push_back(color.y) ;
      colorsList.push_back(color.z) ;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(0));
    glBufferData(GL_ARRAY_BUFFER, positionsList.size() * sizeof(GLfloat), 
            positionsList.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(1));
    glBufferData(GL_ARRAY_BUFFER, colorsList.size() * sizeof(GLfloat), 
            colorsList.data(), GL_STATIC_DRAW);
}

void Mesh::render() const {

    glEnableVertexAttribArray(0);
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

    if (_useIndexing) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_idElementBuffer);
        glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_SHORT,nullptr);
    }
    else {
        glDrawArrays(GL_TRIANGLES,0,_positions.size());
    }
}

const glm::mat4& Mesh::local() const {
    return _local;
}

const glm::mat4& Mesh::world() const {
    return _world;
}

void Mesh::local(const glm::mat4& l) {
    _local = l; 
}

void Mesh::world(const glm::mat4& w) {
    _world = w;
}

void Mesh::updateMatrices(const Ball& b) {
   
    (void) b; //We do not use the rotation state of the ball yet
    std::array<float,3> positionBall = b.get3DPosition();
    _world = glm::translate(glm::mat4(1.f), glm::vec3(positionBall.at(0),
                            positionBall.at(1) ,positionBall.at(2)));
}


Mesh::~Mesh() {
}


