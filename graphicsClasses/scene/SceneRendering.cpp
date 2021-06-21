/*
 * File: SceneRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#include "SceneRendering.h"
#include <future>

SceneRendering::SceneRendering(const Map& map,
                               const Ball& ball,
                               const Star& star,
                               const Camera& camera):
    _starFrame(Frames<StarState>::genFrames<Star>(star)),
    _ballFrame(Frames<BallState>::genFrames<Ball>(ball)),
    _cameraFrame(Frames<CameraState>::genFrames<Camera>(camera)),
    _externalUniformBlocks(createExternalUniformBlockVariables()),
    _externalUniformMatrices(createExternalUniformMatFourVariables()),
    _levelRenderPasses{
                       std::make_shared <RenderPass>(MeshGenerator::genBlocks(map)),
                       std::make_shared <RenderPass>(MeshGenerator::genItems(map)),
                       std::make_shared <RenderPass>(MeshGenerator::genEnemies(map)),
                       std::make_shared <RenderPass>(MeshGenerator::genSpecials(map)),
                       std::make_shared <RenderPass>(MeshGenerator::genBall(ball))
                       },
    _starRenderPass(std::make_shared <RenderPass>(MeshGenerator::genStar(star))),
    _sceneRenderPasses(createSceneRenderPasses()),
    _screenRenderPass(std::make_shared <RenderPass>(MeshGenerator::genQuad(Quad()))),
    _vecScreenRenderPass({ _screenRenderPass }),
    _renderPasses{createRenderPasses()},
    _depthStarProcess(createDepthStarProcess()),
    _sceneRenderingProcess(createSceneRenderingProcess()),
    _brightPassFilterProcess(createBrightPassProcess()),
    _horizontalBlurProcess(createHorizontalBlurProcess()),
    _verticalBlurProcess(createVerticalBlurProcess()),
    _bloomProcess(createBloomProcess()),
    _renderingPipeline{
                       _depthStarProcess,
                       _sceneRenderingProcess,
                       _brightPassFilterProcess,
                       _horizontalBlurProcess,
                       _verticalBlurProcess,
                       _bloomProcess
                       } {

    // TODO: Move thoses instructions
    // alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // -----
    glEnable(GL_CULL_FACE);
}

vecRenderPass_sptr SceneRendering::createSceneRenderPasses() const {
    vecRenderPass_sptr sceneRenderPasses = _levelRenderPasses;
    sceneRenderPasses.push_back(_starRenderPass);
    return sceneRenderPasses;
}

vecRenderPass_sptr SceneRendering::createRenderPasses() const {
    vecRenderPass_sptr renderPasses = _sceneRenderPasses;
    renderPasses.push_back(_screenRenderPass);
    return renderPasses;
}

RenderProcess_sptr SceneRendering::createDepthStarProcess() const {

    const auto createDepthStarShaders =
        [this] ()->RenderProcess::PassShaderMap {
            return {
                {
                    { _levelRenderPasses.at(0), ShaderProgram::createShaderProgram(blocksVs, depthFs) },
                    { _levelRenderPasses.at(1), ShaderProgram::createShaderProgram(itemsMapVs, depthFs) },
                    { _levelRenderPasses.at(2), ShaderProgram::createShaderProgram(enemiesVs, depthFs) },
                    { _levelRenderPasses.at(3), ShaderProgram::createShaderProgram(specialsVs, depthFs) },
                    { _levelRenderPasses.at(4), ShaderProgram::createShaderProgram(ballVs, depthFs) }
                }
            };
        };

    const auto updateDepthStarFct =
        [this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
            const auto& starState = _starFrame->getRenderableState();
            renderPass->upsertUniform(shaderProgramID, VPName, Camera::genVPMatrixFromStar(*starState));
        };

    const auto createDepthStarUniformsUpdating =
        [this, &updateDepthStarFct] () {
            RenderProcess::PassUniformUpdateMap uniformUpdatingFcts;
            for (const RenderPass_sptr& levelRenderPass : _levelRenderPasses) {
                uniformUpdatingFcts[levelRenderPass] = updateDepthStarFct;
            }
            return uniformUpdatingFcts;
        };

    return std::make_shared <RenderProcess>(
        _levelRenderPasses,
        createDepthStarShaders(),
        createDepthStarUniformsUpdating(),
        FrameBuffer_uptr(new FrameBuffer(
                             FrameBuffer::Content::Depth,
                             true,
                             sizeDepthTexture,
                             sizeDepthTexture,
                             true,
                             { 1.f, 1.f, 1.f }))
        );
}

RenderProcess_sptr SceneRendering::createSceneRenderingProcess() const {
    const std::string& lightBlockName = SceneRendering::lightName;
    const auto& lightBlock = getUniformBlock(lightBlockName);

    const auto createSceneRenderingShaders =
        [this, lightBlockName, lightBlock] ()->RenderProcess::PassShaderMap {

            const vecCstShaderProgram_sptr shaderPrograms {
                ShaderProgram::createShaderProgram(blocksVs, levelFs),
                ShaderProgram::createShaderProgram(itemsMapVs, levelFs),
                ShaderProgram::createShaderProgram(enemiesVs, levelFs),
                ShaderProgram::createShaderProgram(specialsVs, levelFs),
                ShaderProgram::createShaderProgram(ballVs, levelFs),
                ShaderProgram::createShaderProgram("starVs.vs", "starFs.fs")
            };
            const std::vector <bool> usingLight = { true, true, true, true, true, false };

            RenderProcess::PassShaderMap passShaderMap;
            for (size_t i = 0; i < _sceneRenderPasses.size(); ++i) {
                const auto& shaderProgram = shaderPrograms.at(i);
                if (usingLight.at(i)) {
                    lightBlock->registerShader(shaderProgram, lightBlockName);
                }
                passShaderMap[_sceneRenderPasses.at(i)] = shaderProgram;
            }
            return passShaderMap;
        };

    const auto updateSceneLevelRenderingFct =
        [this, lightBlock, lightBlockName] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {

            const auto& cameraState = _cameraFrame->getRenderableState();
            renderPass->upsertUniform(shaderProgramID, SceneRendering::VPName, cameraState->viewProjection());
            const std::string& VPStarName = SceneRendering::VPStarName;
            renderPass->upsertUniform(shaderProgramID, VPStarName, getUniformMatrix(VPStarName));
            renderPass->upsertUniform(shaderProgramID, SceneRendering::positionCameraName, cameraState->pos());

            const auto& lightBlock = getUniformBlock(lightBlockName);
            renderPass->upsertUniform(shaderProgramID, lightBlockName, lightBlock);
            renderPass->upsertUniformTexture(
                shaderProgramID,
                "depthTexture",
                _depthStarProcess->getFrameBufferTexture());
        };

    const auto updateSceneStarRenderingFct =
        [this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
            const auto& cameraState = _cameraFrame->getRenderableState();
            renderPass->upsertUniform(shaderProgramID, SceneRendering::VPName, cameraState->viewProjection());
            renderPass->upsertUniform(shaderProgramID, SceneRendering::positionCameraName, cameraState->pos());
        };


    // TODO: this function is duplicated, refactor !
    const auto createSceneRenderingUniformsUpdating =
        [this, &updateSceneLevelRenderingFct, &updateSceneStarRenderingFct] () {
            RenderProcess::PassUniformUpdateMap uniformUpdatingFcts;
            for (const RenderPass_sptr& levelRenderPass : _levelRenderPasses) {
                uniformUpdatingFcts[levelRenderPass] = updateSceneLevelRenderingFct;
            }
            uniformUpdatingFcts[_starRenderPass] = updateSceneStarRenderingFct;
            return uniformUpdatingFcts;
        };

    return std::make_shared <RenderProcess>(
        _sceneRenderPasses,
        createSceneRenderingShaders(),
        createSceneRenderingUniformsUpdating(),
        FrameBuffer_uptr(new FrameBuffer(FrameBuffer::Content::HDR, true))
        );
}

RenderProcess::PassUniformUpdateMap SceneRendering::createScreenSpaceUniformsUpdating (
    const std::map <std::string, RenderProcess_sptr>& textureNameRenderProcess
    ) const {
    const auto uniformsTextureUpdatingFunction =
        [textureNameRenderProcess] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
            for (const auto& textureNameRenderProcessValue : textureNameRenderProcess) {
                const std::string& textureName = textureNameRenderProcessValue.first;
                const RenderProcess_sptr renderProcess = textureNameRenderProcessValue.second;
                renderPass->upsertUniformTexture(
                    shaderProgramID,
                    textureName,
                    renderProcess->getFrameBufferTexture()
                    );
            }
        };
    return {{ _screenRenderPass, uniformsTextureUpdatingFunction }};
}

// TODO refactor those functions
RenderProcess_sptr SceneRendering::createBrightPassProcess() const {
    return std::make_shared <RenderProcess>(
        _vecScreenRenderPass,
        createScreenShaders("brightPassFilter.fs"),
        createBrightPassUniformsUpdating(),
        createScreenSpaceEffectFrameBuffer(FrameBuffer::Content::HDR)
        );
}

RenderProcess_sptr SceneRendering::createHorizontalBlurProcess() const {
    return std::make_shared <RenderProcess>(
        _vecScreenRenderPass,
        createScreenShaders("horizontalBlurFs.fs"),
        createHorizontalBlurUniformsUpdating(),
        createScreenSpaceEffectFrameBuffer(FrameBuffer::Content::HDR)
        );
}

RenderProcess_sptr SceneRendering::createVerticalBlurProcess() const {
    return std::make_shared <RenderProcess>(
        _vecScreenRenderPass,
        createScreenShaders("verticalBlurFs.fs"),
        createVerticalBlurUniformsUpdating(),
        createScreenSpaceEffectFrameBuffer(FrameBuffer::Content::SDR)
        );
}

RenderProcess_sptr SceneRendering::createBloomProcess() const {
    return std::make_shared <RenderProcess>(
        _vecScreenRenderPass,
        createScreenShaders("bloomFs.fs"),
        createBloomUniformsUpdating(),
        nullptr
        );
}

RenderProcess::PassShaderMap SceneRendering::createScreenShaders (const std::string& fs) const {
    return {
        { _screenRenderPass, ShaderProgram::createShaderProgram(basicFboVs, fs) }
    };
}

RenderProcess::PassUniformUpdateMap SceneRendering::createBrightPassUniformsUpdating() const {
    return createScreenSpaceUniformsUpdating({{ "textureScene", _sceneRenderingProcess }});
}

RenderProcess::PassUniformUpdateMap SceneRendering::createHorizontalBlurUniformsUpdating() const {
    return createScreenSpaceUniformsUpdating({{ "brightPassTexture", _brightPassFilterProcess }});
}

RenderProcess::PassUniformUpdateMap SceneRendering::createVerticalBlurUniformsUpdating() const {
    return createScreenSpaceUniformsUpdating({{ "horizontalBlurTexture", _horizontalBlurProcess }});
}

RenderProcess::PassUniformUpdateMap SceneRendering::createBloomUniformsUpdating() const {
    const auto uniformsBloomUpdatingFunction =
        [this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
            const auto& ballState = _ballFrame->getRenderableState();
            renderPass->upsertUniformTexture(
                shaderProgramID,
                "frameSceneHDRTexture",
                _sceneRenderingProcess->getFrameBufferTexture());
            renderPass->upsertUniformTexture(
                shaderProgramID,
                "frameBluredTexture",
                _verticalBlurProcess->getFrameBufferTexture());
            renderPass->upsertUniform(shaderProgramID, "teleportationCoeff",
                                      ballState->teleportationCoeff());
            renderPass->upsertUniform(
                shaderProgramID,
                "flashColor",
                Utility::colorAsVec3(ballState->teleportationColor())
                );
        };
    return {{ _screenRenderPass, uniformsBloomUpdatingFunction }};
}

Rendering::ExternalUniformBlockVariables SceneRendering::createExternalUniformBlockVariables() const {

    const std::string& lightName = SceneRendering::lightName;
    const auto updateBlocksVariablesFct =
        [this, &lightName] (const RenderPass::UniformBlockVariables_uptr& uniformBlocks)->void {
            const auto& starState = _starFrame->getRenderableState();
            uniformBlocks->at(lightName)->update(
                StarState::lightDirectionName,
                Utility::convertToOpenGLFormat(starState->getLightDirection())
                );
        };

    const auto createBlocksVariables =
        [this, &lightName] ()->RenderPass::UniformBlockVariables {
            return  {{
                lightName,
                std::make_shared <UniformLight>(
                    glm::vec3(0.f, 0.f, 0.f),
                    glm::vec3(0.7f, 0.7f, 0.7f),
                    glm::vec3(0.25f, 0.25f, 0.25f),
                    glm::vec3(0.25f, 0.25f, 0.25f))
            }};
        };
    const auto createBlocksVariablesPtr =
        [this, &createBlocksVariables] () {
            return RenderPass::UniformBlockVariables_uptr(
                new RenderPass::UniformBlockVariables(
                    createBlocksVariables())
                );
        };

    return { createBlocksVariablesPtr(), updateBlocksVariablesFct };
}

Rendering::ExternalUniformVariables <glm::mat4> SceneRendering::createExternalUniformMatFourVariables() const
{

    const auto updateMat4VariablesFct =
        [this] (const Mesh::UniformVariables_uptr <glm::mat4>& uniformMatrices) {
            const auto& starState = _starFrame->getRenderableState();
            uniformMatrices->at(VPStarName) = Camera::genVPMatrixFromStar(*starState);
        };

    const auto createMat4Variables =
        [this] ()->Mesh::UniformVariables <glm::mat4> {
            const auto& starState = _starFrame->getRenderableState();
            const auto& cameraState = _cameraFrame->getRenderableState();
            return  {
                { SceneRendering::VPStarName, Camera::genVPMatrixFromStar(*starState) },
                { SceneRendering::VPName, cameraState->viewProjection() }
            };
        };

    const auto createMat4VariablesPtr =
        [this, &createMat4Variables] () {
            return Mesh::UniformVariables_uptr <glm::mat4>(
                new Mesh::UniformVariables <glm::mat4>(createMat4Variables())
                );

        };

    return { createMat4VariablesPtr(), updateMat4VariablesFct };
}

// TODO should be in FrameBuffer class
FrameBuffer_uptr SceneRendering::createScreenSpaceEffectFrameBuffer (
    const FrameBuffer::Content& content
    ) const {
    return FrameBuffer_uptr(new FrameBuffer(
                                content,
                                false,
                                Utility::getWidthFromHeight(heightBloomTexture),
                                heightBloomTexture,
                                false
                                ));
}

const UniformBlock_sptr& SceneRendering::getUniformBlock (const std::string& name) const {
    return _externalUniformBlocks.uniformBlockVariables->at(name);
}

const glm::mat4& SceneRendering::getUniformMatrix (const std::string& name) const {
    return _externalUniformMatrices.uniformVariables->at(name);
}

void SceneRendering::update() {
    //Todo refactor
    _starFrame->getUpdatableState()->update();
    _ballFrame->getUpdatableState()->update();
    _cameraFrame->getUpdatableState()->update();

    const auto& uniformBlocks = _externalUniformBlocks.uniformBlockVariables;
    const auto& updatingBlocksFct = _externalUniformBlocks.uniformBlockVariablesUpdatingFct;
    updatingBlocksFct(uniformBlocks);

    const auto& uniformMatrices = _externalUniformMatrices.uniformVariables;
    const auto& updatingMatricesFct = _externalUniformMatrices.uniformVariablesUpdatingFct;
    updatingMatricesFct(uniformMatrices);

    for (const auto& renderPass : _renderPasses) {
        renderPass->update();
    }

    for (const auto& renderProcess : _renderingPipeline) {
        renderProcess->updateUniforms();
    }
}

void SceneRendering::swapFrames() {
    _starFrame->swapFrames();
    _ballFrame->swapFrames();
    _cameraFrame->swapFrames();
    for (auto sceneRenderPass: _sceneRenderPasses) {
        sceneRenderPass->swapFrames();
    }
}

void SceneRendering::render() const {
    for (const auto& renderProcess : _renderingPipeline) {
        renderProcess->render();
    }
}

const std::string SceneRendering::blocksVs = "blocksVs.vs";
const std::string SceneRendering::itemsMapVs = "itemsMapVs.vs";
const std::string SceneRendering::enemiesVs = "enemiesVs.vs";
const std::string SceneRendering::specialsVs = "specialsVs.vs";
const std::string SceneRendering::ballVs = "ballVs.vs";
const std::string SceneRendering::basicFboVs = "basicFboVs.vs";
const std::string SceneRendering::levelFs = "levelFs.fs";
const std::string SceneRendering::depthFs = "depthFs.fs";

const std::string SceneRendering::VPName = "VP";
const std::string SceneRendering::VPStarName = "VPStar";
const std::string SceneRendering::positionCameraName = "positionCamera";
const std::string SceneRendering::lightName = "light";
