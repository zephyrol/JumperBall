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
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "Camera.h"


class Rendering {

public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Rendering                       ( const Map& map, 
                                      const Ball& ball, 
                                      const Camera& camera );
    virtual                         ~Rendering();


    //------------TYPES------------//
    enum class Attribute            { Positions, Normals, Colors, UVCoords };
    

    //------------METHODS----------//
    void render() ; 
    
private:

    //------------TYPES------------//
    template<typename T>
    using verticesAttributeData =   std::map<Attribute,std::vector<T> >;

    template<typename T>
    using uniformValue =            std::map<std::string,T >;
    

    //--------ATTRIBUTES-----------//
    uniformValue<glm::mat4>         _uniformMatrix4;
    uniformValue<glm::vec4>         _uniformVec4;
    uniformValue<glm::vec3>         _uniformVec3;
    uniformValue<glm::vec2>         _uniformVec2;
    uniformValue<GLfloat>           _uniformFloat;
    uniformValue<bool>              _uniformBool;

    Mesh                            _meshMap;
    Mesh                            _meshBall;

    const Map&                      _map;
    const Ball&                     _ball;
    const Camera&                   _camera;

    const ShaderProgram             _spMap;
    GLuint                          _idVertexArray;
    std::array<GLuint,2>            _idVertexBuffer;
    verticesAttributeData<GLfloat>  _vData;


    //------------METHODS----------//
    verticesAttributeData<GLfloat>  mapVertices ();
    void                            renderMap();
    void                            renderCamera();



    //--------STATIC METHODS-------//
    static const std::map<Attribute,unsigned int> 
                                    nbComponents;
    static const std::string        vsshaderMap; 
    static const std::string        fsshaderMap; 

};

#endif /* RENDERING_H */

