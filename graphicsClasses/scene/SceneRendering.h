/*
 * File: SceneRendering.h
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#ifndef SCENE_RENDERING_H
#define SCENE_RENDERING_H
#include <iostream>
#include <map>
#include <vector>
#include "Camera.h"
#include "uniformBlocks/uniformLight.h"
#include "componentsGeneration/MeshGenerator.h"
#include "../Rendering.h"


class SceneRendering:public Rendering {

public:
SceneRendering(const Map& map, const Ball& ball, const Star& star, const Camera& camera);

void update();
void render() const;

private:
template<typename T> using uniformVariable = std::map <std::string, T>;

static constexpr size_t heightBloomTexture = 192;
static constexpr size_t sizeDepthTexture = 1024;

const Camera& _camera;

// const std::shared_ptr <UniformLight> _light;

CstState_sptr _starState;
// glm::mat4 _VPStar;

// const vecRenderPass_sptr _renderPasses;
// const RenderProcess_sptr _depthStarProcess;
const RenderProcess_sptr _sceneRenderingProcess;
const RenderProcess_sptr _brightPassFilterProcess;
const RenderProcess_sptr _horizontalBlurProcess;
const RenderProcess_sptr _verticalBlurProcess;
const RenderProcess_sptr _bloomProcess;
// const vecRenderProcess_sptr _sceneRenderingPipeline;

const RenderPass_sptr& getScreenRenderPass() const;
std::vector <RenderPass_sptr> vecScreenRenderPass() const;
vecRenderPass_sptr getLevelRenderPasses() const;
const RenderPass_sptr& getStarRenderPass() const;
vecRenderPass_sptr getSceneRenderPasses() const;


RenderProcess_sptr createDepthStarProcess() const;
RenderProcess_sptr createSceneRenderingProcess() const;

RenderProcess::PassUniformUpdateMap createScreenSpaceUniformsUpdating(
    const std::map <std::string, RenderProcess_sptr>& textureNameRenderProcess
    ) const;
RenderProcess_sptr createBrightPassProcess() const;
RenderProcess_sptr createHorizontalBlurProcess() const;
RenderProcess_sptr createVerticalBlurProcess() const;
RenderProcess_sptr createBloomProcess() const;

// RenderProcess::PassShaderMap createDepthStarShaders() const;
RenderProcess::PassShaderMap createSceneRenderingShaders() const;

RenderProcess::PassShaderMap createScreenShaders(const std::string& fs) const;
RenderProcess::PassUniformUpdateMap createSceneRenderingUniformsUpdating() const;

RenderProcess::PassUniformUpdateMap createBrightPassUniformsUpdating() const;
RenderProcess::PassUniformUpdateMap createHorizontalBlurUniformsUpdating() const;
RenderProcess::PassUniformUpdateMap createVerticalBlurUniformsUpdating() const;
RenderProcess::PassUniformUpdateMap createBloomUniformsUpdating() const;

Rendering::ExternalUniformBlockVariables createExternalUniformBlockVariables() const;
Rendering::ExternalUniformVariables <glm::mat4> createExternalUniformMatFourVariables() const;

void updateCameraUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateCameraUniformsStar(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;

glm::mat4 genVPStar() const;

FrameBuffer_uptr createScreenSpaceEffectFrameBuffer(const FrameBuffer::TextureCaterory& category) const;
CstShaderProgram_uptr createBloomEffectShader(const std::string& vs, const std::string& fs) const;

static const std::string blocksVs;
static const std::string objectsMapVs;
static const std::string enemiesVs;
static const std::string ballVs;
static const std::string basicFboVs;
static const std::string levelFs;
static const std::string depthFs;
};

#endif /* SCENE_RENDERING_H */
