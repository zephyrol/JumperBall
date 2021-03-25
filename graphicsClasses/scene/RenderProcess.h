/*
 * File: RenderProcess.h
 * Author: Morgenthaler S
 *
 * Created on 25 mars 2021, 21:48
 */

#include "RenderPass.h"
#include "FrameBuffer.h"

class RenderProcess {

public:
    RenderProcess(
      ShaderProgram&& shaderProgram,
      FrameBuffer&& frameBuffer,
      const vecRenderPass_sptr& renderPasses
    );
    RenderProcess(const RenderProcess& renderProcess) = delete;
    RenderProcess& operator=(const RenderProcess&) = delete;

    RenderProcess(RenderProcess&& renderProcess) = default;
    RenderProcess& operator=(RenderProcess&&) = default;

    ~RenderProcess() = default; 

private:
    const ShaderProgram _shaderProgram;
    const FrameBuffer _frameBuffer;
    const vecRenderPass_sptr _renderPasses;

};
