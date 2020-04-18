/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Rendering.h
 * Author: Morgenthaler S
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


    //------------METHODS----------//
    void                            render() ; 
    
private:

    //------------TYPES------------//

    template<typename T>
    using uniformVariable           = std::map<std::string,T >;
    
    //---------CONSTANTS------------//

    static constexpr float          blurSigma                             = 4.f;
    static constexpr size_t         blurPatchSize                         = 25;
    static constexpr size_t         heightBloomTexture                    = 192;
    static constexpr float          scaleBloomTexture =     heightBloomTexture /
                                               static_cast<float>(RESOLUTION_Y);

    //--------ATTRIBUTES-----------//
    uniformVariable<glm::mat4>      _uniformMatrix4;
    uniformVariable<glm::vec4>      _uniformVec4;
    uniformVariable<glm::vec3>      _uniformVec3;
    uniformVariable<glm::vec2>      _uniformVec2;
    uniformVariable<GLfloat>        _uniformFloat;
    uniformVariable<bool>           _uniformBool;

    const Quad                      _quadFrame;
    Mesh<Map>                       _meshMap;
    Mesh<Ball>                      _meshBall;
    Mesh<Star>                      _meshStar;
    Mesh<Quad>                      _meshQuadFrame;

    const Ball&                     _ball;
    const Star&                     _star;
    const Camera&                   _camera;

    UniformLight                    _light;


    const ShaderProgram             _spMap;
    const ShaderProgram             _spStar;
    const ShaderProgram             _spFbo;

    const ShaderProgram             _spBlur;
    const ShaderProgram             _spToneMapping;
    const ShaderProgram             _spBrightPassFilter;
    const ShaderProgram             _spBloom;

    const FrameBuffer               _frameBufferScene;
    const FrameBuffer               _frameBufferToneMapping;
    const FrameBuffer               _frameBufferHalfBlur;
    const FrameBuffer               _frameBufferCompleteBlur;
    const FrameBuffer               _frameBufferBrightPassFilter;
    const FrameBuffer               _frameBufferBloom;


    //------------METHODS----------//
    void                            blurEffect( 
                                               const FrameBuffer& referenceFBO);
    void                            toneMappingEffect( 
                                               const FrameBuffer& referenceFBO);
    void                            brightPassEffect( 
                                               const FrameBuffer& referenceFBO);
    void                            bloomEffect(  const FrameBuffer& fboScene,
                                                  const FrameBuffer& fboLight );

    void                            renderCamera(const ShaderProgram& sp);



    //------STATIC ATTRIBUTES------//
    static const std::string        vsshaderMap; 
    static const std::string        fsshaderMap; 
    static const std::string        vsshaderStar; 
    static const std::string        fsshaderStar; 
    static const std::string        vsshaderFBO; 
    static const std::string        fsshaderFBO; 
    static const std::string        vsshaderBlur; 
    static const std::string        fsshaderBlur; 
    static const std::string        vsshaderToneMapping; 
    static const std::string        fsshaderToneMapping; 
    static const std::string        vsshaderBrightPassFilter; 
    static const std::string        fsshaderBrightPassFilter; 
    static const std::string        vsshaderBloom; 
    static const std::string        fsshaderBloom; 

    static const std::vector<float> gaussComputedValues;
};

#endif /* RENDERING_H */

