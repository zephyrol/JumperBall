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
#include "Star.h"


class Rendering {

public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Rendering                       ( const Map& map, 
                                      const Ball& ball, 
                                      const Star& star, 
                                      const Camera& camera );
    virtual                         ~Rendering();


    //------------METHODS----------//
    void render() ; 
    
private:

    //------------TYPES------------//

    template<typename T>
    using uniformVariable           = std::map<std::string,T >;
    

    //--------ATTRIBUTES-----------//
    uniformVariable<glm::mat4>      _uniformMatrix4;
    uniformVariable<glm::vec4>      _uniformVec4;
    uniformVariable<glm::vec3>      _uniformVec3;
    uniformVariable<glm::vec2>      _uniformVec2;
    uniformVariable<GLfloat>        _uniformFloat;
    uniformVariable<bool>           _uniformBool;

    Mesh                            _meshMap;
    Mesh                            _meshBall;

    const Map&                      _map;
    const Ball&                     _ball;
    const Star&                     _star;
    const Camera&                   _camera;

    const ShaderProgram             _spMap;
    const ShaderProgram             _spStar;


    //------------METHODS----------//
    void                            renderMap();
    void                            renderCamera(const ShaderProgram& sp);

    void                            bindUniform(const std::string&    name,
                                                const glm::mat4&      value,
                                                const ShaderProgram&  sp);

    void                            bindUniform(const std::string&    name,
                                                const glm::vec4&      value,
                                                const ShaderProgram&  sp);

    void                            bindUniform(const std::string&    name,
                                                const glm::vec3&      value,
                                                const ShaderProgram&  sp);

    void                            bindUniform(const std::string&    name,
                                                const glm::vec2&      value,
                                                const ShaderProgram&  sp);

    void                            bindUniform(const std::string&    name,
                                                const GLfloat&        value,
                                                const ShaderProgram&  sp);

    void                            bindUniform(const std::string&    name,
                                                const bool&           value,
                                                const ShaderProgram&  sp);

    //------STATIC ATTRIBUTES------//
    static const std::string        vsshaderMap; 
    static const std::string        fsshaderMap; 
    static const std::string        vsshaderStar; 
    static const std::string        fsshaderStar; 

};

#endif /* RENDERING_H */

