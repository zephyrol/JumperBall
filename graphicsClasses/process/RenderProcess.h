/*
 * File: RenderProcess.h
 * Author: Morgenthaler S
 *
 * Created on 25 mars 2021, 21:48
 */

#ifndef RENDER_PROCESS_H
#define RENDER_PROCESS_H
#include "Utility.h"
#include "ShaderProgram.h"
#include "frameBuffer/TextureSampler.h"

class RenderProcess;

using RenderProcess_sptr = std::shared_ptr<RenderProcess>;
using CstRenderProcess_sptr = std::shared_ptr<const RenderProcess>;
using vecCstRenderProcess_sptr = std::vector<CstRenderProcess_sptr>;
using vecRenderProcess_sptr = std::vector<RenderProcess_sptr>;

/**
 * Render process is used to render a specific effect. It may render it into frame buffer of
 * directly on the screen. It contains a specific shader program.
 */
class RenderProcess {

public:

    RenderProcess(const RenderProcess &renderProcess) = delete;

    RenderProcess() = default;

    RenderProcess &operator=(const RenderProcess &) = delete;

    virtual ~RenderProcess() = default;

    virtual void update();

    virtual void render() const = 0;

    virtual const CstTextureSampler_uptr &getRenderTexture() const = 0;
};

#endif