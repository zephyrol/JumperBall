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
#include "scene/MeshGenerator.h"
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

const Quad _quadScreen;
const Camera& _camera;

const std::shared_ptr <UniformLight> _light;

StarState _starState;
const glm::vec3 _envSize;
const glm::vec3 _centerWorld;
const float _largestSize;
glm::mat4 _VPStar;

const vecRenderPass_sptr _renderPasses;
const RenderProcess_sptr _depthStarProcess;
const RenderProcess_sptr _sceneRenderingProcess;
const RenderProcess_sptr _brightPassFilterProcess;
const RenderProcess_sptr _horizontalBlurProcess;
const RenderProcess_sptr _verticalBlurProcess;
const RenderProcess_sptr _bloomProcess;
const vecRenderProcess_sptr _sceneRenderingPipeline;

const RenderPass_sptr& getScreenRenderPass() const;
std::vector <RenderPass_sptr> vecScreenRenderPass() const;
vecRenderPass_sptr getLevelRenderPasses() const;
const RenderPass_sptr& getStarRenderPass() const;
vecRenderPass_sptr getSceneRenderPasses() const;

RenderProcess::PassShaderMap createDepthStarShaders() const;
RenderProcess::PassShaderMap createSceneRenderingShaders() const;

RenderProcess::PassShaderMap createScreenShaders(const std::string& vs, const std::string& fs) const;
RenderProcess::PassShaderMap createBrightPassShaders() const;
RenderProcess::PassShaderMap createHorizontalBlurShaders() const;
RenderProcess::PassShaderMap createVerticalBlurShaders() const;
RenderProcess::PassShaderMap createBloomShaders() const;

RenderProcess::PassUniformUpdateMap createDepthStarUniforms() const;
RenderProcess::PassUniformUpdateMap createSceneRenderingUniforms() const;

RenderProcess::PassUniformUpdateMap createScreenUniforms(
    const std::function <void(const RenderPass_sptr&, GLuint)>& uniformsUpdatingFunction
    ) const;
RenderProcess::PassUniformUpdateMap createBrightPassUniforms() const;
RenderProcess::PassUniformUpdateMap createHorizontalBlurUniforms() const;
RenderProcess::PassUniformUpdateMap createVerticalBlurUniforms() const;
RenderProcess::PassUniformUpdateMap createBloomUniforms() const;

void updateDepthUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateCameraUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateCameraUniformsStar(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateBrightPassFilterUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateHorizontalBlurUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateVerticalBlurUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;
void updateBloomUniforms(const RenderPass_sptr& renderPass, GLuint shaderProgramID) const;

float getLargestSize() const;
glm::mat4 genVPStar() const;

FrameBuffer_uptr createBloomEffectFrameBuffer(const FrameBuffer::TextureCaterory& category) const;
CstShaderProgram_uptr createBloomEffectShader(const std::string& vs, const std::string& fs) const;

static const std::string blocksVs;
static const std::string objectsMapVs;
static const std::string ballVs;
static const std::string basicFboVs;
static const std::string levelFs;
static const std::string depthFs;
};

#endif /* SCENE_RENDERING_H */
