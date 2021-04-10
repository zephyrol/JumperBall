/*
 * File: Rendering.h
 * Author: Morgenthaler S
 *
 * Created on 9 avril 2021, 22h45
 */

#ifndef RENDERING_H
#define RENDERING_H
#include "scene/RenderProcess.h"

class Rendering {

public:
using ExternalUniformBlockVariables = std::map <
    RenderPass::UniformBlockVariables_uptr,
    std::function <void (const RenderPass::UniformBlockVariables_uptr&)>
    >;
template<typename T> using ExternalUniformVariables = std::map <
    Mesh::UniformVariables_uptr <T>,
    std::function <void (const Mesh::UniformVariables_uptr <T>&)>
    >;

Rendering(
    const vecState_sptr& externalStates,
    ExternalUniformBlockVariables&& _externalUniformBlocks,
    ExternalUniformVariables <glm::mat4>&& externalUniformMatrices,
    const vecRenderPass_sptr& renderPasses,
    const vecRenderProcess_sptr& renderingPipeline
    );
Rendering(const Rendering& rendering) = delete;
Rendering& operator= (const Rendering& rendering) = delete;

void update();
void render() const;

private:
const vecState_sptr _externalStates;
ExternalUniformBlockVariables _externalUniformBlocks;
ExternalUniformVariables <glm::mat4> _externalUniformMatrices;
const vecRenderPass_sptr _renderPasses;
const vecRenderProcess_sptr _renderingPipeline;

};


#endif /* RENDERING_H */
