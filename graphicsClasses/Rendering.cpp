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

const std::string Rendering::vsshaderMap = "graphicsClasses/shaders/basicVs.vs";
const std::string Rendering::fsshaderMap = "graphicsClasses/shaders/basicFs.fs";


Rendering::Rendering(const Map& map, const Ball& ball, const Camera& camera):
    _uniformMatrix4(),
    _uniformVec4(),
    _uniformVec3(),
    _uniformVec2(),
    _vData(),
    _map(map),
    _ball(ball),
    _camera(camera),
    _spMap( Shader (GL_VERTEX_SHADER, vsshaderMap ),
            Shader (GL_FRAGMENT_SHADER, fsshaderMap ))
{
}

Rendering::verticesAttributeData<GLfloat> Rendering::mapVertices() {
    Rendering::verticesAttributeData<GLfloat> data;
    return data;
}

void Rendering::render() const {

}

void Rendering::renderMap() {
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
                    _vData.at(Rendering::Attribute::Positions).insert(
                    _vData.at(Rendering::Attribute::Positions).end(),
                            pCube.begin(), pCube.end()
                    );

                    std::vector<GLfloat> cCube=Utility::getColorsLocalCube();
                    _vData.at(Rendering::Attribute::Colors).insert(
                    _vData.at(Rendering::Attribute::Colors).end(),
                            cCube.begin(), cCube.end()
                    );

                }
            }
        }
    }
}


Rendering::~Rendering() {
}

