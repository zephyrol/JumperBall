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
#include "FrameBuffer.h"
#include "Mesh.h"
#include "Camera.h"
#include "Star.h"
#include "uniformBlocks/uniformLight.h"
#include "animations/BallAnimation.h"


class Rendering {

public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Rendering                       ( const Map& map, 
                                      const Ball& ball, 
                                      const Star& star, 
                                      const Camera& camera );
    virtual                         ~Rendering();


    //------------METHODS----------//
    void                            render() ; 
    
private:

    //------------TYPES------------//

    template<typename T>
    using uniformVariable           = std::map<std::string,T >;
    
    //---------CONSTANTS------------//

    static constexpr float          blurSigma                             = 4.f;
    static constexpr size_t         blurPatchSize                         = 9;

    //--------ATTRIBUTES-----------//
    uniformVariable<glm::mat4>      _uniformMatrix4;
    uniformVariable<glm::vec4>      _uniformVec4;
    uniformVariable<glm::vec3>      _uniformVec3;
    uniformVariable<glm::vec2>      _uniformVec2;
    uniformVariable<GLfloat>        _uniformFloat;
    uniformVariable<bool>           _uniformBool;

    Mesh                            _meshMap;
    Mesh                            _meshBall;
    Mesh                            _meshQuadFrame;

    const Map&                      _map;

    const Ball&                     _ball;
    BallAnimation                   _ballAnimation;

    const Star&                     _star;
    UniformLight                    _light;

    const Camera&                   _camera;

    const ShaderProgram             _spMap;
    const ShaderProgram             _spStar;
    const ShaderProgram             _spFbo;

    const ShaderProgram             _spBlur;
    const ShaderProgram             _spBrightPassFilter;

    const FrameBuffer               _frameBufferScene;
    const FrameBuffer               _frameBufferHalfBlur;
    const FrameBuffer               _frameBufferCompleteBlur;
    const FrameBuffer               _frameBufferBrightPassFilter;


    //------------METHODS----------//
    void                            renderMap();
    void                            blurEffect( 
                                                const FrameBuffer& referenceFBO);
    void                            brightPassEffect( 
                                                const FrameBuffer& referenceFBO);
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

    void                            bindUniform(const std::string&    name,
                                                const int&            value,
                                                const ShaderProgram&  sp);

    void                            bindUniform(const std::string&    name,
                                                const std::vector<int>& value,
                                                const ShaderProgram&  sp);

    void                            bindUniform(const std::string&    name,
                                                const std::vector<float>& value,
                                                const ShaderProgram&  sp);

    void                            bindUniformTexture
                                                (const std::string&    name,
                                                const int&             value,
                                                const ShaderProgram&  sp);


    //------STATIC ATTRIBUTES------//
    static const std::string        vsshaderMap; 
    static const std::string        fsshaderMap; 
    static const std::string        vsshaderStar; 
    static const std::string        fsshaderStar; 
    static const std::string        vsshaderFBO; 
    static const std::string        fsshaderFBO; 
    static const std::string        vsshaderBlur; 
    static const std::string        fsshaderBlur; 
    static const std::string        vsshaderBrightPassFilter; 
    static const std::string        fsshaderBrightPassFilter; 

    static const std::vector<float> gaussComputedValues;
};

#endif /* RENDERING_H */

