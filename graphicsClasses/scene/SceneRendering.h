/*
 * File: SceneRendering.h
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#ifndef SCENE_RENDERING_H
#define SCENE_RENDERING_H
#include "componentsGeneration/MeshGenerator.h"
#include "Rendering.h"
#include "Scene.h"
#include "process/scene/ShadowProcess.h"
#include "process/scene/LevelProcess.h"
#include "process/scene/BrightPassFilterProcess.h"
#include "process/scene/HorizontalBlurProcess.h"
#include "process/scene/VerticalBlurProcess.h"
#include "process/scene/BloomProcess.h"
#include "process/scene/SceneUniformBuffer.h"


class SceneRendering:public Rendering {

public:
SceneRendering(
    const Scene& scene,
    GLsizei width,
    GLsizei height,
    GLint defaultFrameBuffer,
    const JBTypes::FileContent& fileContent
    );

void update() override;
void freeGPUMemory() override;

void render() const override;

private:

static constexpr size_t heightBloomTexture = 192;

const GLsizei _expensivePreprocessWidth;
const GLsizei _expensivePreprocessHeight;

const Scene& _scene;

const RenderPass_sptr _blocks;
const RenderPass_sptr _items;
const RenderPass_sptr _enemies;
const RenderPass_sptr _specials;
const RenderPass_sptr _ball;
const RenderPass_sptr _star;
const RenderPass_sptr _screen;
const vecRenderPass_sptr _renderPasses;

const std::shared_ptr<ShadowProcess> _shadowStar;
const std::shared_ptr<LevelProcess> _sceneRenderingProcess;
const std::shared_ptr<BrightPassFilterProcess> _brightPassFilter;
const std::shared_ptr<HorizontalBlurProcess> _horizontalBlur;
const std::shared_ptr<VerticalBlurProcess> _verticalBlur;
const std::shared_ptr<BloomProcess> _bloom;
const vecRenderProcess_sptr _processes;

SceneUniformBuffer _sceneUniformBuffer;

vecCstShaderProgram_sptr getShaderProgramsUsingUniformBuffer() const;

static const std::string VPName;
static const std::string VPStarName;
static const std::string cameraPositionName;
static const std::string lightName;
};

#endif /* SCENE_RENDERING_H */
