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
#include "CameraState.h"
#include "uniformBlocks/uniformLight.h"
#include "componentsGeneration/MeshGenerator.h"
#include "Rendering.h"


class SceneRendering:public Rendering {

public:
SceneRendering(const Map& map, const Ball& ball, const Star& star, const Camera& camera);

void update() override;
void render() const override;

private:

static constexpr size_t heightBloomTexture = 192;
static constexpr size_t sizeDepthTexture = 1024;


const std::shared_ptr <StarState> _starState;
const std::shared_ptr <BallState> _ballState;
const std::shared_ptr <CameraState> _cameraState;
const vecState_sptr _externalStates;

ExternalUniformBlockVariables _externalUniformBlocks;
ExternalUniformVariables <glm::mat4> _externalUniformMatrices;

const vecRenderPass_sptr _levelRenderPasses;
const RenderPass_sptr _starRenderPass;
const vecRenderPass_sptr _sceneRenderPasses;
const RenderPass_sptr _screenRenderPass;
const vecRenderPass_sptr _vecScreenRenderPass;
const vecRenderPass_sptr _renderPasses;

const RenderProcess_sptr _depthStarProcess;
const RenderProcess_sptr _sceneRenderingProcess;
const RenderProcess_sptr _brightPassFilterProcess;
const RenderProcess_sptr _horizontalBlurProcess;
const RenderProcess_sptr _verticalBlurProcess;
const RenderProcess_sptr _bloomProcess;
const vecRenderProcess_sptr _renderingPipeline;
vecRenderPass_sptr createSceneRenderPasses() const;
vecRenderPass_sptr createRenderPasses() const;

RenderProcess_sptr createDepthStarProcess() const;
RenderProcess_sptr createSceneRenderingProcess() const;

RenderProcess_sptr createBrightPassProcess() const;
RenderProcess_sptr createHorizontalBlurProcess() const;
RenderProcess_sptr createVerticalBlurProcess() const;
RenderProcess_sptr createBloomProcess() const;

RenderProcess::PassShaderMap createScreenShaders(const std::string& fs) const;

RenderProcess::PassUniformUpdateMap createScreenSpaceUniformsUpdating(
    const std::map <std::string, RenderProcess_sptr>& textureNameRenderProcess
    ) const;

RenderProcess::PassUniformUpdateMap createBrightPassUniformsUpdating() const;
RenderProcess::PassUniformUpdateMap createHorizontalBlurUniformsUpdating() const;
RenderProcess::PassUniformUpdateMap createVerticalBlurUniformsUpdating() const;
RenderProcess::PassUniformUpdateMap createBloomUniformsUpdating() const;

Rendering::ExternalUniformVariables <glm::mat4> createExternalUniformMatFourVariables() const;
Rendering::ExternalUniformBlockVariables createExternalUniformBlockVariables() const;

const UniformBlock_sptr& getUniformBlock(const std::string& name) const;
const glm::mat4& getUniformMatrix(const std::string& name) const;

FrameBuffer_uptr createScreenSpaceEffectFrameBuffer(const FrameBuffer::Content& category) const;

static const std::string VPName;
static const std::string VPStarName;
static const std::string positionCameraName;
static const std::string lightName;

static const std::string blocksVs;
static const std::string itemsMapVs;
static const std::string enemiesVs;
static const std::string specialsVs;
static const std::string ballVs;
static const std::string basicFboVs;
static const std::string levelFs;
static const std::string depthFs;
};

#endif /* SCENE_RENDERING_H */
