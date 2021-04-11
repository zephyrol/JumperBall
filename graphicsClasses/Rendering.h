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

using UniformBlockUpdatingFct = std::function <void (const RenderPass::UniformBlockVariables_uptr&)>;
using ExternalUniformBlockVariables = std::map <
    RenderPass::UniformBlockVariables_uptr,
    UniformBlockUpdatingFct
    >;

template<typename T> using UniformVariableUpdatingFct = std::function <
    void (const Mesh::UniformVariables_uptr <T>&)
    >;
template<typename T> using ExternalUniformVariables = std::map <
    Mesh::UniformVariables_uptr <T>,
    UniformVariableUpdatingFct <T>
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

protected:
CstState_sptr getExternalState(size_t number) const;
const glm::mat4& getUniformMatrix(size_t number) const;
const RenderPass_sptr& getRenderPass(size_t number) const;

private:
const vecState_sptr _externalStates;
ExternalUniformBlockVariables _externalUniformBlocks;
ExternalUniformVariables <glm::mat4> _externalUniformMatrices;
const vecRenderPass_sptr _renderPasses;
const vecRenderProcess_sptr _renderingPipeline;

};


#endif /* RENDERING_H */
