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
#include "RenderGroupsManager.h"

class RenderPass;

using RenderPass_sptr = std::shared_ptr<RenderPass>;
using CstRenderPass_sptr = std::shared_ptr<const RenderPass>;
using vecCstRenderPass_sptr = std::vector<CstRenderPass_sptr>;
using vecRenderPass_sptr = std::vector<RenderPass_sptr>;

/**
 * Render passes is used to render meshes with a specific shader.
 * During rendering, every render groups (through render groups manager) are rendered with the same shader.
 * The render groups manager may be shared with other render passes.
 */
class RenderPass {

public:

    explicit RenderPass(
        CstShaderProgram_sptr shaderProgram,
        CstRenderGroupsManager_sptr renderGroupsManager
    );

    RenderPass(const RenderPass &renderPass) = delete;

    RenderPass &operator=(const RenderPass &renderPass) = delete;

    RenderPass(RenderPass &&renderPass) = default;

    void render() const;

    void update();

private:

    const CstShaderProgram_sptr _shaderProgram;
    const CstRenderGroupsManager_sptr _renderGroupsManager;

    /**
     * This pointer contains a list of render group reflecting the group manager state.
     * The uniforms needs to be recreated if the rendering manager has changed its definition
     * of its render group. This pointer is used to check if manager has made a redefinition.
     */
    std::shared_ptr<const std::vector<RenderGroup>> _currentRenderGroups;

    std::vector<MeshUniforms> _uniforms;

};

#endif /* RENDER_PASS_H */
