/*
 * File: RenderProcess.h
 * Author: Morgenthaler S
 *
 * Created on 25 mars 2021, 21:48
 */

#include "process/RenderPass.h"
#include "FrameBuffer.h"
#include <functional>

class RenderProcess;
using RenderProcess_sptr = std::shared_ptr <RenderProcess>;
using CstRenderProcess_sptr = std::shared_ptr <const RenderProcess>;
using vecCstRenderProcess_sptr = std::vector <CstRenderProcess_sptr>;
using vecRenderProcess_sptr = std::vector <RenderProcess_sptr>;

class RenderProcess {

public:

RenderProcess(const RenderProcess& renderProcess) = delete;

    RenderProcess() = default;

    RenderProcess& operator= (const RenderProcess&) = delete;

~RenderProcess() = default;

virtual void render() const = 0;
virtual void freeGPUMemory() = 0;
virtual std::shared_ptr<const GLuint> getRenderTexture() const = 0;
};
