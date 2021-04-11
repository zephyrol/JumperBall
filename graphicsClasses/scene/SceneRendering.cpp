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
    Rendering(
        { std::make_shared <StarState>(star) },
        std::move(createExternalUniformBlockVariables()),
        std::move(createExternalUniformMatFourVariables()),
{
    std::make_shared <RenderPass>(MeshGenerator::genBlocks(map)),
    std::make_shared <RenderPass>(MeshGenerator::genObjects(map)),
    std::make_shared <RenderPass>(MeshGenerator::genEnemies(map)),
    std::make_shared <RenderPass>(MeshGenerator::genBall(ball)),
    std::make_shared <RenderPass>(MeshGenerator::genStar(star)),
    std::make_shared <RenderPass>(MeshGenerator::genQuad(Quad()))
},
        { createDepthStarProcess(),
          createBrightPassProcess(),
          createHorizontalBlurProcess(),
          createVerticalBlurProcess(),
          createBloomProcess()
        }),
    _camera(camera),
    _starState(getExternalState(0)),
    _VPStar(genVPStar()) {
    /*_sceneRenderingProcess(std::make_shared <RenderProcess>(
                               getSceneRenderPasses(),
                               createSceneRenderingShaders(),
                               createSceneRenderingUniformsUpdating(),
                               FrameBuffer_uptr(new FrameBuffer(FrameBuffer::TextureCaterory::HDR, true))
                               )),*/
    update();

    // alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // -----
    glEnable(GL_CULL_FACE);
}

RenderProcess::PassShaderMap SceneRendering::createSceneRenderingShaders() const {

    RenderProcess::PassShaderMap sceneRenderingShaders;
    const auto fillSceneRenderingShader =
        [this, &sceneRenderingShaders] ( const RenderPass_sptr& renderPass,
                                         const std::string& vertexShader,
                                         const std::string& fragmentShader,
                                         bool useLight)->void {
            CstShaderProgram_uptr shaderProgram = ShaderProgram::createShaderProgram(vertexShader,
                                                                                     fragmentShader);
            if (useLight) {
                _light->registerShader(shaderProgram);
            }
            sceneRenderingShaders[renderPass] = std::move(shaderProgram);
        };

    const vecRenderPass_sptr levelRenderPasses = getLevelRenderPasses();
    fillSceneRenderingShader(levelRenderPasses.at(0), blocksVs, levelFs, true);
    fillSceneRenderingShader(levelRenderPasses.at(1), objectsMapVs, levelFs, true);
    fillSceneRenderingShader(levelRenderPasses.at(2), enemiesVs, levelFs, true);
    fillSceneRenderingShader(levelRenderPasses.at(3), ballVs, levelFs, true);

    fillSceneRenderingShader(getStarRenderPass(), "starVs.vs", "starFs.fs", false);
    return sceneRenderingShaders;
}

const RenderPass_sptr& SceneRendering::getScreenRenderPass() const {
    return getRenderPass(5);
}

std::vector <RenderPass_sptr> SceneRendering::vecScreenRenderPass() const {
    return { getScreenRenderPass() };
}

vecRenderPass_sptr SceneRendering::getLevelRenderPasses() const {
    return { getRenderPass(0), getRenderPass(1), getRenderPass(2), getRenderPass(3) };
}

const RenderPass_sptr& SceneRendering::getStarRenderPass() const {
    return getRenderPass(4);
}

vecRenderPass_sptr SceneRendering::getSceneRenderPasses() const {
    vecRenderPass_sptr sceneRenderPasses = getLevelRenderPasses();
    sceneRenderPasses.push_back(getStarRenderPass());
    return sceneRenderPasses;
}

RenderProcess_sptr SceneRendering::createDepthStarProcess() const {

    const vecRenderPass_sptr levelRenderPasses = getLevelRenderPasses();

    const auto createDepthStarShaders =
        [&levelRenderPasses] ()->RenderProcess::PassShaderMap {
            return {
                {
                    { levelRenderPasses.at(0), ShaderProgram::createShaderProgram(blocksVs, depthFs) },
                    { levelRenderPasses.at(1), ShaderProgram::createShaderProgram(objectsMapVs, depthFs) },
                    { levelRenderPasses.at(2), ShaderProgram::createShaderProgram(enemiesVs, depthFs) },
                    { levelRenderPasses.at(3), ShaderProgram::createShaderProgram(ballVs, depthFs) }
                }
            };
        };

    const auto updateDepthStarFct = [this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
                                        renderPass->upsertUniform(shaderProgramID, "VP", _VPStar);
                                    };

    const auto createDepthStarUniformsUpdating =
        [&levelRenderPasses, &updateDepthStarFct] () {
            RenderProcess::PassUniformUpdateMap uniformUpdatingFcts;
            for (const RenderPass_sptr& levelRenderPass : levelRenderPasses) {
                uniformUpdatingFcts[levelRenderPass] = updateDepthStarFct;
            }
            return uniformUpdatingFcts;
        };

    return std::make_shared <RenderProcess>(
        levelRenderPasses,
        createDepthStarShaders(),
        createDepthStarUniformsUpdating(),
        FrameBuffer_uptr(new FrameBuffer(
                             FrameBuffer::TextureCaterory::Depth,
                             true,
                             sizeDepthTexture,
                             sizeDepthTexture,
                             true,
                             { 1.f, 1.f, 1.f }))
        );
}

RenderProcess::PassUniformUpdateMap SceneRendering::createScreenSpaceUniformsUpdating (
    const std::map <std::string, RenderProcess_sptr>& textureNameRenderProcess
    ) const {

    const auto uniformsTextureUpdatingFunction =
        [&textureNameRenderProcess] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
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

    return {{ getScreenRenderPass(), uniformsTextureUpdatingFunction }};
}

RenderProcess_sptr SceneRendering::createBrightPassProcess() const {
    return std::make_shared <RenderProcess>(
        vecScreenRenderPass(),
        createScreenShaders("brightPassFilter.fs"),
        createBrightPassUniformsUpdating(),
        createScreenSpaceEffectFrameBuffer(FrameBuffer::TextureCaterory::HDR)
        );
}

RenderProcess_sptr SceneRendering::createHorizontalBlurProcess() const {
    return std::make_shared <RenderProcess>(
        vecScreenRenderPass(),
        createScreenShaders("horizontalBlurFs.fs"),
        createHorizontalBlurUniformsUpdating(),
        createScreenSpaceEffectFrameBuffer(FrameBuffer::TextureCaterory::HDR)
        );
}

RenderProcess_sptr SceneRendering::createVerticalBlurProcess() const {
    return std::make_shared <RenderProcess>(
        vecScreenRenderPass(),
        createScreenShaders("verticalBlurFs.fs"),
        createHorizontalBlurUniformsUpdating(),
        createScreenSpaceEffectFrameBuffer(FrameBuffer::TextureCaterory::SDR)
        );
}

RenderProcess_sptr SceneRendering::createBloomProcess() const {
    return std::make_shared <RenderProcess>(
        vecScreenRenderPass(),
        createScreenShaders("bloomFs.fs"),
        createBloomUniformsUpdating(),
        nullptr
        );
}


RenderProcess::PassShaderMap SceneRendering::createScreenShaders (const std::string& fs) const {
    return {
        { getScreenRenderPass(), ShaderProgram::createShaderProgram(basicFboVs, fs) }
    };
}

RenderProcess::PassUniformUpdateMap SceneRendering::createSceneRenderingUniformsUpdating() const {

    RenderProcess::PassUniformUpdateMap uniformUpdatingFcts;
    const auto updateCam =
        [this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
            this->updateCameraUniforms(renderPass, shaderProgramID);
        };
    const auto updateCamStar =
        [this] (const RenderPass_sptr& renderPass, GLuint shaderProgramID)->void {
            this->updateCameraUniformsStar(renderPass, shaderProgramID);
        };

    for (const RenderPass_sptr& levelRenderPass : getLevelRenderPasses()) {
        uniformUpdatingFcts[levelRenderPass] = updateCam;
    }
    uniformUpdatingFcts[getStarRenderPass()] = updateCamStar;
    return uniformUpdatingFcts;
}


RenderProcess::PassUniformUpdateMap SceneRendering::createBrightPassUniformsUpdating() const {
    return createScreenSpaceUniformsUpdating(
        {{ "textureScene", _sceneRenderingProcess }});
}

RenderProcess::PassUniformUpdateMap SceneRendering::createHorizontalBlurUniformsUpdating() const {
    return createScreenSpaceUniformsUpdating(
        {{ "brightPassTexture", _brightPassFilterProcess }});
}

RenderProcess::PassUniformUpdateMap SceneRendering::createVerticalBlurUniformsUpdating() const {
    return createScreenSpaceUniformsUpdating(
        {{ "horizontalBlurTexture", _horizontalBlurProcess }});
}

RenderProcess::PassUniformUpdateMap SceneRendering::createBloomUniformsUpdating() const {
    return createScreenSpaceUniformsUpdating(
    {
        { "frameSceneHDRTexture", _sceneRenderingProcess },
        { "frameBluredTexture", _verticalBlurProcess },
    });
}

Rendering::ExternalUniformBlockVariables SceneRendering::createExternalUniformBlockVariables() const {
    const std::string lightName = "light";

    const auto updateBlocksVariablesFct =
        [this, &lightName] (const RenderPass::UniformBlockVariables_uptr& uniformBlocks)->void {
            uniformBlocks->at(lightName)->update(
                StarState::lightDirectionName,
                Utility::convertToOpenGLFormat(_starState->getExposableVec3f(StarState::lightDirectionName))
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

    return {{ createBlocksVariablesPtr(), updateBlocksVariablesFct }};
}

Rendering::ExternalUniformVariables <glm::mat4> SceneRendering::createExternalUniformMatFourVariables() const
{
    const std::string VPStarName = "VPStar";

    const auto updateMat4VariablesFct =
        [this, &VPStarName] (const Mesh::UniformVariables_uptr <glm::mat4>& uniformMatrices) {
            uniformMatrices->at(VPStarName) = genVPStar();
        };

    const auto createMat4Variables = [this, &VPStarName] ()->Mesh::UniformVariables <glm::mat4> {
                                         return  {{ VPStarName, genVPStar() }};
                                     };

    const auto createMat4VariablesPtr = [this, &createMat4Variables] () {
                                            return Mesh::UniformVariables_uptr <glm::mat4>(
                                                new Mesh::UniformVariables <glm::mat4>(createMat4Variables())
                                                );
                                        };

    return [this, &createMat4VariablesPtr,
            &updateMat4VariablesFct] ()->Rendering::ExternalUniformVariables <glm::mat4> {
               return {{ createMat4VariablesPtr(), updateMat4VariablesFct }};
           }();
}

void SceneRendering::updateCameraUniforms (const RenderPass_sptr& renderPass, GLuint shaderProgramID) const {
    renderPass->upsertUniform(shaderProgramID, "VP", _camera.viewProjection());
    renderPass->upsertUniform(shaderProgramID, "VPStar", _VPStar);
    renderPass->upsertUniform(shaderProgramID, "positionCamera", _camera.pos());

    renderPass->upsertUniform(shaderProgramID, _light->name(), _light);
    renderPass->upsertUniformTexture(
        shaderProgramID,
        "depthTexture",
        _depthStarProcess->getFrameBufferTexture()
        );
}

void SceneRendering::updateCameraUniformsStar (
    const RenderPass_sptr& renderPass,
    GLuint shaderProgramID) const {
    renderPass->upsertUniform(shaderProgramID, "VP", _camera.viewProjection());
    renderPass->upsertUniform(shaderProgramID, "positionCamera", _camera.pos());
}

glm::mat4 SceneRendering::genVPStar() const {

    constexpr float offsetJumpingBall = 1.f; // size of ball + jump height
    const float envSize = _starState->getExposableFloat(StarState::envSizeName);
    const float halfBoundingBoxSize = envSize / 2.f +
                                      offsetJumpingBall;

    // We use a close star position to get a better ZBuffer accuracy
    const JBTypes::vec3f& rotationCenter = _starState->getExposableVec3f(StarState::rotationCenterName);
    const glm::vec3 centerWorld = Utility::convertToOpenGLFormat(rotationCenter);

    const JBTypes::vec3f& position = _starState->getExposableVec3f(StarState::positionName);
    const glm::vec3 closeStarPosition = centerWorld + glm::normalize(
        (Utility::convertToOpenGLFormat(position)) - centerWorld
        )  * halfBoundingBoxSize;

    return glm::ortho(
        -halfBoundingBoxSize,
        halfBoundingBoxSize,
        -halfBoundingBoxSize,
        halfBoundingBoxSize,
        _camera.zNear, _camera.zNear + 2.f * halfBoundingBoxSize
        ) * glm::lookAt(closeStarPosition, centerWorld, glm::vec3(0.f, 1.f, 0.f));
}


// TODO should be in FrameBuffer class
FrameBuffer_uptr SceneRendering::createScreenSpaceEffectFrameBuffer (
    const FrameBuffer::TextureCaterory& category
    ) const {
    return FrameBuffer_uptr(new FrameBuffer(
                                category,
                                false,
                                Utility::getWidthFromHeight(heightBloomTexture),
                                heightBloomTexture,
                                false
                                ));
}

/*void SceneRendering::update() {
    _starState.update();
    _VPStar = genVPStar();
    _light->directionLight(Utility::convertToOpenGLFormat(_starState.lightDirection()));
    _light->update();

    for (auto& renderPass : _renderPasses) {
        renderPass->update();
    }
    for (auto& renderProcess : _sceneRenderingPipeline) {
        renderProcess->updateUniforms();
    }
   }

   void SceneRendering::render() const {

    for (const auto& renderProcess : _sceneRenderingPipeline) {
        renderProcess->render();
    }

   }*/

const std::string SceneRendering::blocksVs = "blocksVs.vs";
const std::string SceneRendering::objectsMapVs = "objectsMapVs.vs";
const std::string SceneRendering::enemiesVs = "enemiesVs.vs";
const std::string SceneRendering::ballVs = "ballVs.vs";
const std::string SceneRendering::basicFboVs = "basicFboVs.vs";
const std::string SceneRendering::levelFs = "levelFs.fs";
const std::string SceneRendering::depthFs = "depthFs.fs";
