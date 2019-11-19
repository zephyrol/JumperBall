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



    uint     iParaCount  = 40;
    uint     iMeriCount  = 60;
    float    r           = 0.5f;
    
    // Create a sphere ---------------------------------------------------------
    GLuint iVertexCount = iParaCount * iMeriCount;
    
    _positions.reserve(iVertexCount);
    _uvCoords.reserve(iVertexCount);
    
    float a1 = ( 180.0 / ( iParaCount + 1 ) ) * M_PI / 180.0;
    float a2 = ( 360.0 / ( iMeriCount - 1 ) ) * M_PI / 180.0;
    
    // parallels ---------------------------------------------------------------
    uint k = 0;
    for( uint i = 0; i < iParaCount; ++i )
    {
        float fAngle    = - static_cast<float>(M_PI) / 2.0f + a1 * ( i + 1 );
        float z         = r * static_cast<float>(sin( fAngle ));
        float fRadius   = r * static_cast<float>(cos( fAngle ));
        
        for( uint j = 0; j < iMeriCount; ++j )
        {
            _positions[ k ] = glm::vec3( 
                    fRadius * static_cast<float>(cos( a2 * j )), 
                    fRadius * static_cast<float>(sin( a2 * j )), 
                    z );
            _uvCoords[ k ] = glm::vec2( float( j )/ iMeriCount, 
                    float( iParaCount - i ) / iParaCount );
            k++;
        }
    }
    // compute normals ---------------------------------------------------------
    // on a 0 centered sphere : you just need to normalise the position!
    _normals.reserve(iVertexCount);
    
    for( uint i = 0; i < iVertexCount; ++i )
    {
        _normals[ i ] = glm::normalize( _positions[ i ] );
    }
    
    GLuint iElementsCount = ( iMeriCount - 1 ) * ( iParaCount - 1 ) * 2 * 3; 
    // for quads split in 2
    
    _indices.reserve(iElementsCount);
    
    k=0;
    for( uint i = 0; i < ( iParaCount - 1 ); ++i )
    {
        for( uint j = 0; j < ( iMeriCount - 1 ); ++j )
        {
            _indices[ k++ ] = iMeriCount * i + j;
            _indices[ k++ ] = iMeriCount * i + ( j + 1 );
            _indices[ k++ ] = iMeriCount * ( i + 1 ) + ( j + 1 );
            _indices[ k++ ] = iMeriCount * ( i + 1 ) + ( j + 1 );
            _indices[ k++ ] = iMeriCount * ( i + 1 ) + j;
            _indices[ k++ ] = iMeriCount * i + j;
        }
    }
   
    std::array<float,3> positionBall = ball.get3DPos();
    glm::translate(_world, glm::vec3(positionBall.at(1),positionBall.at(2)
                            ,positionBall.at(3)));

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
      
    }
    else {
    glDrawArrays(GL_TRIANGLES,0,_positions.size());
    }
}


Mesh::~Mesh() {
}

