/*
 * File: SceneRendering.h
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#ifndef RENDERING_H
#define RENDERING_H
#include <iostream>
#include <map>
#include <vector>
#include "FrameBuffer.h"
#include "Camera.h"
#include "uniformBlocks/uniformLight.h"
#include "animations/BallAnimation.h"
#include "scene/mesh/MeshGenerator.h"
#include "RenderPass.h"


class SceneRendering {

public:

SceneRendering(const Map& map, const Ball& ball, const Star& star, const Camera& camera);

void update();
void render() const;

private:
template<typename T> using uniformVariable = std::map <std::string, T>;

static constexpr float blurSigma = 4.f;
static constexpr size_t blurPatchSize = 25;
static constexpr size_t heightBloomTexture = 192;
static constexpr size_t sizeDepthTexture = 1024;

uniformVariable <glm::mat4> _uniformMatrix4;
uniformVariable <glm::vec4> _uniformVec4;
uniformVariable <glm::vec3> _uniformVec3;
uniformVariable <glm::vec2> _uniformVec2;
uniformVariable <GLfloat> _uniformFloat;
uniformVariable <bool> _uniformBool;

// const Quad _quadFrame;
// MapMesh _meshMap;
// BallMesh _meshBall;
// StarMesh _meshStar;
// QuadMesh _meshQuadFrame;

// ParallelTask <void> _meshMapUpdate;
// ParallelTask <void> _meshBallUpdate;
// ParallelTask <void> _meshStarUpdate;
ParallelTask <void> _uniformUpdate;

const Camera& _camera;

const ShaderProgram _spBlocks;
const ShaderProgram _spObjects;
const ShaderProgram _spStar;
const ShaderProgram _spBall;
const ShaderProgram _spFbo;
const ShaderProgram _spBlur;
const ShaderProgram _spBrightPassFilter;
const ShaderProgram _spBloom;
const ShaderProgram _spDepth;

UniformLight _light;

const FrameBuffer _frameBufferDepth;
const FrameBuffer _frameBufferHDRScene;
const FrameBuffer _frameBufferBrightPassEffect;
const FrameBuffer _frameBufferHalfBlurEffect;
const FrameBuffer _frameBufferCompleteBlurEffect;

MapState _mapState;
RenderPass _renderPassBlocks;
RenderPass _renderPassObjects;

StarState _starState;
RenderPass _renderPassStar;

RenderPass _renderPassBall;

// ---------CONST METHODS--------//
void phongEffect(GLuint depthTexture) const;
void blurEffect(GLuint brightPassTexture) const;
void brightPassEffect(GLuint hdrSceneTexture) const;
void bloomEffect(GLuint hdrSceneTexture, GLuint bluredTexture) const;
void depthFromStar() const;

void bindCamera(const ShaderProgram& sp) const;
void bindStarView(const ShaderProgram& sp) const;

// ------------METHODS----------//
void updateUniform();
void updateCamera(RenderPass& renderPass);

// ------STATIC ATTRIBUTES------//
static const std::string vsshaderBlocks;
static const std::string fsshaderBlocks;
static const std::string vsshaderObjects;
static const std::string fsshaderObjects;
static const std::string vsshaderBall;
static const std::string fsshaderBall;
static const std::string vsshaderStar;
static const std::string fsshaderStar;
static const std::string vsshaderFBO;
static const std::string fsshaderFBO;
static const std::string vsshaderBlur;
static const std::string fsshaderBlur;
static const std::string vsshaderBrightPassFilter;
static const std::string fsshaderBrightPassFilter;
static const std::string vsshaderBloom;
static const std::string fsshaderBloom;
static const std::string vsshaderDepth;
static const std::string fsshaderDepth;

static const std::vector <float> gaussComputedValues;
static const glm::vec3 backgroundColor;
};

#endif /* SCENE_RENDERING_H */
