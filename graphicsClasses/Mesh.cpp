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


Mesh::Mesh(const Ball& ball):
      _positions(),
      _normals(),
      _colors(),
      _uvCoords(),
      _useIndexing(true),
      _indices(),
      _local(1.f)
{
  (void) ball;
}

Mesh::Mesh(const Map& map):
      _positions(),
      _normals(),
      _colors(),
      _uvCoords(),
      _useIndexing(false),
      _indices(),
      _local(1.f)
{

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

Mesh::~Mesh() {
}

