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

using UniformUpdatingFct = std::function <void (const RenderPass_sptr&)>;
RenderProcess(
    const vecRenderPass_sptr& renderPasses,
    std::map <RenderPass_sptr, CstShaderProgram_uptr>&& shaderPrograms,
    std::map <RenderPass_sptr, UniformUpdatingFct>&& externalUniformsTreating,
    FrameBuffer_uptr&& frameBuffer);
RenderProcess(const RenderProcess& renderProcess) = delete;
RenderProcess& operator= (const RenderProcess&) = delete;

~RenderProcess() = default;

void updateUniforms();
void render() const;

private:
const vecRenderPass_sptr _renderPasses;
const std::map <RenderPass_sptr, CstShaderProgram_uptr> _shaderPrograms;
const std::map <RenderPass_sptr, UniformUpdatingFct> _externalUniformsTreating;
const FrameBuffer_uptr _frameBuffer;
};
