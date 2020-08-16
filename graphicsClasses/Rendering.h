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
    static constexpr float          blurSigma                            = 4.f;
    static constexpr size_t         blurPatchSize                        = 25;
    static constexpr size_t         heightBloomTexture                   = 192;
    static constexpr size_t         sizeDepthTexture                     = 1024;

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
    const ShaderProgram             _spBrightPassFilter;
    const ShaderProgram             _spBloom;
    const ShaderProgram             _spDepth;

    const FrameBuffer               _frameBufferDepth;
    const FrameBuffer               _frameBufferHDRScene;
    const FrameBuffer               _frameBufferBrightPassEffect;
    const FrameBuffer               _frameBufferHalfBlurEffect;
    const FrameBuffer               _frameBufferCompleteBlurEffect;

    //------------METHODS----------//
    void                            phongEffect(GLuint depthTexture);
    void                            blurEffect(GLuint brightPassTexture) const;
    //void                            toneMappingEffect(
                                               //const FrameBuffer& referenceFBO);
    void                            brightPassEffect(GLuint hdrSceneTexture);
    void                            bloomEffect(GLuint hdrSceneTexture,
                                                GLuint bluredTexture );
    void                            depthFromStar();

    void                            bindCamera(const ShaderProgram& sp);
    void                            bindStarView(const ShaderProgram& sp);


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
    static const std::string        vsshaderBloom;
    static const std::string        fsshaderBloom;
    static const std::string        vsshaderDepth;
    static const std::string        fsshaderDepth;

    static const std::vector<float> gaussComputedValues;
    static const glm::vec3          backgroundColor;
};

#endif /* RENDERING_H */

