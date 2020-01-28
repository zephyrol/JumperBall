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
#include "glm/gtx/transform.hpp"


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
      _world(1.f)
{
  
    glGenVertexArrays(1, &_idVertexArray);
    glBindVertexArray(_idVertexArray);

    glGenBuffers(3, _idVertexBuffer.data());


    constexpr unsigned int  iParaCount  = 40;
    constexpr unsigned int  iMeriCount  = 60;
    const     float         r           = ball.getRadius();
    
    // Create a sphere ---------------------------------------------------------
    GLuint iVertexCount = iParaCount * iMeriCount;
    
    constexpr float a1 = ( 180.0 / ( iParaCount - 1 ) ) * M_PI / 180.0;
    constexpr float a2 = ( 360.0 / ( iMeriCount - 1 ) ) * M_PI / 180.0;
    
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
            _colors.push_back(glm::vec3(static_cast<float>(i)/iParaCount,
                                        (j < iMeriCount/2) ? 1.f : 0.f,
                                        0.5f));
        }
    }
    // compute normals ---------------------------------------------------------
    // on a 0 centered sphere : you just need to normalise the position!
    _normals.reserve(iVertexCount);
    
    for( unsigned int i = 0; i < iVertexCount; ++i )
    {
        _normals.push_back(glm::normalize( _positions[ i ] ));
    }
    
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
   
    JumperBallTypes::vec3f positionBall = ball.get3DPosition();
    _world = glm::translate(_world, glm::vec3(positionBall.x,
                            positionBall.y ,positionBall.z));


    bindVertexData();

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
      _world(1.f)
{

    glGenVertexArrays(1, &_idVertexArray);
    glBindVertexArray(_idVertexArray);

    glGenBuffers(3, _idVertexBuffer.data());


    for (unsigned int x = 0; x < map.boundingBoxXMax() ; ++x ) {
        for (unsigned int y = 0; y < map.boundingBoxYMax() ; ++y ) {
            for (unsigned int z = 0; z < map.boundingBoxZMax() ; ++z ) {
                auto block = map.map3DData(x,y,z);
                if (block) {

                    const std::vector<GLfloat>& pCube= Utility::positionsCube;
                    const std::vector<GLfloat>& cCube= Utility::colorsCube;
                    const std::vector<GLfloat>& nCube= Utility::normalsCube;

                    for (unsigned int i = 0 ; i < pCube.size(); i += 3 )
                    {
                        const GLfloat xAfterTranslation =  
                                      pCube.at(i) + static_cast<GLfloat> (x);
                        const GLfloat yAfterTranslation =  
                                      pCube.at(i+1) + static_cast<GLfloat> (y);
                        const GLfloat zAfterTranslation =  
                                      pCube.at(i+2) + static_cast<GLfloat> (z);

                        _positions.push_back( glm::vec3(xAfterTranslation,
                                yAfterTranslation , zAfterTranslation));
                        _colors.push_back(
                          glm::vec3(cCube.at(i),cCube.at(i+1) ,cCube.at(i+2)));
                        _normals.push_back(
                          glm::vec3(nCube.at(i),nCube.at(i+1) ,nCube.at(i+2)));
                    }
                }
            }
        }
    }

    for (unsigned int x = 0; x < map.boundingBoxXMax() ; ++x ) {
        for (unsigned int y = 0; y < map.boundingBoxYMax() ; ++y ) {
            for (unsigned int z = 0; z < map.boundingBoxZMax() ; ++z ) {
                auto block = map.map3DData(x,y,z);
                if (block) {
                    genSharps(*block,glm::vec3(x,y,z));
                }
            }
        }
    }
    //Gen sharps
    
    bindVertexData();
}

void Mesh::bindVertexData() const {

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

    std::vector<GLfloat> normalsList;
    for (glm::vec3 normal: _normals) {
        normalsList.push_back(normal.x) ;
        normalsList.push_back(normal.y) ;
        normalsList.push_back(normal.z) ;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(0));
    glBufferData(GL_ARRAY_BUFFER, positionsList.size() * sizeof(GLfloat), 
            positionsList.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(1));
    glBufferData(GL_ARRAY_BUFFER, colorsList.size() * sizeof(GLfloat), 
            colorsList.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(2));
    glBufferData(GL_ARRAY_BUFFER, normalsList.size() * sizeof(GLfloat), 
            normalsList.data(), GL_STATIC_DRAW);

}

void Mesh::draw(bool drawAll, unsigned int offset, unsigned int number) const {

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
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

    glBindBuffer(GL_ARRAY_BUFFER, _idVertexBuffer.at(2));
    glVertexAttribPointer ( 
        2,
        3, // 3 GL_FLOAT per vertex
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
        );

    if (_useIndexing) {
        glBindBuffer  (GL_ELEMENT_ARRAY_BUFFER,_idElementBuffer);
        glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_SHORT,nullptr);
    }
    else {
        if (drawAll){
          glDrawArrays(GL_TRIANGLES,offset,_positions.size()-offset);
        }
        else {
          glDrawArrays(GL_TRIANGLES,offset,number);
        }
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}


const glm::mat4& Mesh::world() const {
    return _world;
}

void Mesh::world(const glm::mat4& w) {
    _world = w;
}

void Mesh::updateMatrices(const Ball& b) {
   
    JumperBallTypes::vec3f positionBall = b.get3DPosition();
    _world = glm::translate(glm::mat4(1.f), glm::vec3(positionBall.x,
                            positionBall.y ,positionBall.z));
}

void Mesh::genSharps(const Block& block, glm::vec3 posWorld) {
    
    if (block.getType() == Block::categoryOfBlocksInFile::Sharp) {
        
        const std::array<float,7> scales 
         {.2f,.1f,.05f,.1f,.075f,.15f,0.175f};

        const std::array<glm::vec2,7> translationFloorFactor 
        {
            glm::vec2(0.f,0.f),   glm::vec2(-0.6f,-0.4f), glm::vec2(0.6f,-0.6f),
            glm::vec2(0.2f,0.6f), glm::vec2(-0.2f,-0.6f), glm::vec2(0.6f,0.6f),
            glm::vec2(-0.6f,0.6f)
        };
        
        for(size_t i = 0; i < block.faceInfo().size(); i++) {
            
            bool isSharp = block.faceInfo().at(i);
            if (isSharp) {
                
                constexpr float sizeBlock = 1.f;
                constexpr float offset = sizeBlock/2.f;
                
                JumperBallTypes::Direction currentDir =
                        JumperBallTypesMethods::integerAsDirection(i);
                JumperBallTypes::vec3f vecDir = 
                JumperBallTypesMethods::directionAsVector(currentDir);
                
                glm::mat4 translationOffset = glm::translate(
                        glm::vec3( -offset, 0 , -offset ));
                
                glm::mat4 rotationLocal = 
                        Utility::rotationUpToDir(currentDir);

                
                for (size_t j = 0 ; j < scales.size() ; j++) {

                    glm::mat4 scaleLocal = 
                          glm::scale(glm::vec3(scales.at(j),0.5f,scales.at(j)));
                    
                    //for ()

                    glm::mat4 translationLocal =
                            glm::translate( glm::vec3(
                            posWorld.x + offset + vecDir.x * offset,
                            posWorld.y + offset + vecDir.y * offset,
                            posWorld.z + offset + vecDir.z * offset
                            ));
                    
                    glm::mat4 translationFloor = glm::translate( glm::vec3(
                                        offset * translationFloorFactor.at(j).x,
                                        0.f,
                                        offset * translationFloorFactor.at(j).y
                                        ));
                    
                    const std::vector<GLfloat>& positions  = 
                                                        Utility::positionsPike;
                    const std::vector<GLfloat>& colors     = 
                                                        Utility::colorsPike;
                    const std::vector<GLfloat>& normals    = 
                                                        Utility::normalsPike;
                    
                    for (unsigned int k = 0; k < positions.size(); k+=3)
                    {
                        glm::vec4 position  { positions.at(k),positions.at(k+1),
                                              positions.at(k+2), 1.f};
                        glm::vec4 normal    { normals.at(k), normals.at(k+1),
                                              normals.at(k+2), 1.f};
                        glm::vec3 color     { colors.at(k), colors.at(k+1), 
                                              colors.at(k+2)};
                        
                        position  = translationLocal *  rotationLocal * 
                                     translationFloor * scaleLocal * 
                                    translationOffset * position;
                        normal    = glm::normalize( rotationLocal * scaleLocal * 
                                                    normal);

                        _positions.push_back(glm::vec3(position));
                        _normals.push_back(glm::vec3(normal));
                        _colors.push_back(color);
                    }
                } 
            }
        }
    }
}


Mesh::~Mesh() {
}


