/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Rendering.h
 * Author: seb
 *
 * Created on 6 novembre 2019, 20:38
 */

#ifndef RENDERING_H
#define RENDERING_H
#include <Types.h>
#include <iostream>
#include <map>
#include <vector>
#include <Map.h>
#include <Ball.h>
#include <Camera.h>
#include "Shader.h"
#include "ShaderProgram.h"

class Rendering {
public:
    Rendering(const Map& map, const Ball& ball, const Camera& camera);
    virtual ~Rendering();

    enum class Attribute { Positions, Normals, Colors, UVCoords };
    
    void render() const ; 
    
private:


    template<typename T>
    using verticesAttributeData = std::map<Attribute,std::vector<T> >;
    
    static const std::map<Attribute,unsigned int> nbComponents;
    static const std::string vsshaderMap; 
    static const std::string fsshaderMap; 

    verticesAttributeData<GLfloat>  _vData;

    const Map&                      _map;
    const Ball&                     _ball;
    const Camera&                   _camera;

    verticesAttributeData<GLfloat> mapVertices ();

    const ShaderProgram _spMap;
};

#endif /* RENDERING_H */

