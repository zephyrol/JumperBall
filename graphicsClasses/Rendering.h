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
#include <glm/glm.hpp>
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

    template<typename T>
    using uniformValue = std::map<std::string,T >;
    
    static const std::map<Attribute,unsigned int> nbComponents;
    static const std::string vsshaderMap; 
    static const std::string fsshaderMap; 

    uniformValue<glm::mat4> _uniformMatrix4;
    uniformValue<glm::vec4> _uniformVec4;
    uniformValue<glm::vec4> _uniformVec3;
    uniformValue<glm::vec4> _uniformVec2;

    verticesAttributeData<GLfloat>  _vData;

    const Map&                      _map;
    const Ball&                     _ball;
    const Camera&                   _camera;

    const ShaderProgram _spMap;

    verticesAttributeData<GLfloat> mapVertices ();
    void renderMap();

};

#endif /* RENDERING_H */

