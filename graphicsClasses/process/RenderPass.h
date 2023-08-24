/*
 * File: RenderPass.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 15:00
 */

#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "frameBuffer/FrameBuffer.h"
#include "RenderGroup.h"

class RenderPass;

using RenderPass_sptr = std::shared_ptr<RenderPass>;
using CstRenderPass_sptr = std::shared_ptr<const RenderPass>;
using vecCstRenderPass_sptr = std::vector<CstRenderPass_sptr>;
using vecRenderPass_sptr = std::vector<RenderPass_sptr>;

/**
 * Render passes is used to render meshes with a specific shader.
 * During rendering, the render group is rendered with the shader.
 * The render group may be shared with other render passes.
 */
class RenderPass {

public:

    explicit RenderPass(
        CstShaderProgram_sptr shaderProgram,
        CstRenderGroup_sptr renderGroup
    );

    RenderPass(const RenderPass &renderPass) = delete;

    RenderPass &operator=(const RenderPass &renderPass) = delete;

    RenderPass(RenderPass &&renderPass) = default;

    void renderGroup() const;

    void updateGroupUniforms();

    void bindGroup() const;

private:

    const CstShaderProgram_sptr _shaderProgram;
    const CstRenderGroup_sptr _renderGroup;
    RenderGroupUniforms _groupUniforms;
};

#endif /* RENDER_PASS_H */
