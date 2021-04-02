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
#include "Camera.h"
#include "uniformBlocks/uniformLight.h"
#include "scene/mesh/MeshGenerator.h"
#include "RenderProcess.h"


class SceneRendering {

public:

SceneRendering(const Map& map, const Ball& ball, const Star& star, const Camera& camera);

void update();
void render() const;

private:
template<typename T> using uniformVariable = std::map <std::string, T>;

static constexpr size_t heightBloomTexture = 192;
static constexpr size_t sizeDepthTexture = 1024;

/*uniformVariable <glm::mat4> _uniformMatrix4;
   uniformVariable <glm::vec4> _uniformVec4;
   uniformVariable <glm::vec3> _uniformVec3;
   uniformVariable <glm::vec2> _uniformVec2;
   uniformVariable <GLfloat> _uniformFloat;
   uniformVariable <bool> _uniformBool;*/

// ParallelTask <void> _meshMapUpdate;
// ParallelTask <void> _meshBallUpdate;
// ParallelTask <void> _meshStarUpdate;
// ParallelTask <void> _uniformUpdate;

const Quad _quadScreen;
const Camera& _camera;

/*const ShaderProgram _spBlocks;
   const ShaderProgram _spObjects;
   const ShaderProgram _spStar;
   const ShaderProgram _spBall;
   const ShaderProgram _spFbo;
   const ShaderProgram _spBlur;
   const ShaderProgram _spBrightPassFilter;
   const ShaderProgram _spBloom;
   const ShaderProgram _spDepth;*/

const std::shared_ptr <UniformLight> _light;

/*const FrameBuffer _frameBufferDepth;
   const FrameBuffer _frameBufferHDRScene;
   const FrameBuffer _frameBufferBrightPassEffect;
   const FrameBuffer _frameBufferHalfBlurEffect;
   const FrameBuffer _frameBufferCompleteBlurEffect;*/

// MapState _mapState;
// RenderPass _renderPassBlocks;
// RenderPass _renderPassObjects;

const vecRenderPass_sptr _renderPasses;
StarState _starState;

const RenderProcess_sptr _sceneRenderingProcess;
const RenderProcess_sptr _brightPassFilterProcess;
const RenderProcess_sptr _horizontalBlurProcess;
const RenderProcess_sptr _verticalBlurProcess;
const RenderProcess_sptr _bloomProcess;
const vecRenderProcess_sptr _sceneRenderingPipeline;
// RenderProcess _bloomEffectProcess;
// RenderPass _renderPassStar;

// RenderPass _renderPassBall;

const RenderPass_sptr& getScreenRenderPass() const;
std::vector <RenderPass_sptr> vecScreenRenderPass() const;
vecRenderPass_sptr getLevelRenderPasses() const;
const RenderPass_sptr& getStarRenderPass() const;
vecRenderPass_sptr getSceneRenderPasses() const;

RenderProcess::PassShaderMap createSceneRenderingShaders() const;

RenderProcess::PassShaderMap createScreenShaders(const std::string& vs, const std::string& fs) const;
RenderProcess::PassShaderMap createBrightPassShaders() const;
RenderProcess::PassShaderMap createHorizontalBlurShaders() const;
RenderProcess::PassShaderMap createVerticalBlurShaders() const;
RenderProcess::PassShaderMap createBloomShaders() const;

RenderProcess::PassUniformUpdateMap createSceneRenderingUniforms() const;

RenderProcess::PassUniformUpdateMap createScreenUniforms(
    const std::function <void(const RenderPass_sptr&, GLuint)>& uniformsUpdatingFunction
    ) const;
RenderProcess::PassUniformUpdateMap createBrightPassUniforms() const;
RenderProcess::PassUniformUpdateMap createHorizontalBlurUniforms() const;
RenderProcess::PassUniformUpdateMap createVerticalBlurUniforms() const;
RenderProcess::PassUniformUpdateMap createBloomUniforms() const;

void phongEffect(GLuint depthTexture) const;
void blurEffect(GLuint brightPassTexture) const;
void brightPassEffect(GLuint hdrSceneTexture) const;
void bloomEffect(GLuint hdrSceneTexture, GLuint bluredTexture) const;
void depthFromStar() const;

void bindCamera(const ShaderProgram& sp) const;
void bindStarView(const ShaderProgram& sp) const;

void updateUniform();
void updateCameraUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateCameraUniformsStar(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateBrightPassFilterUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateHorizontalBlurUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateVerticalBlurUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateBloomUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;

FrameBuffer_uptr createBloomEffectFrameBuffer(const FrameBuffer::TextureCaterory& category) const;
CstShaderProgram_uptr createBloomEffectShader(const std::string& vs, const std::string& fs) const;

// ------STATIC ATTRIBUTES------//
/*static const std::string vsshaderBlocks;
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
   static const std::string fsshaderDepth;*/

// static const std::vector <float> gaussComputedValues;
};

#endif /* SCENE_RENDERING_H */
