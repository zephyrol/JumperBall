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


Rendering::~Rendering() {
}

