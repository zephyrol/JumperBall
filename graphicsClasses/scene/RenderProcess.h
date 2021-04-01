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

using UniformUpdatingFct = std::function <void (const RenderPass_sptr&, GLuint)>;
using PassShaderMap = std::map <RenderPass_sptr, CstShaderProgram_uptr>;
using PassUniformUpdateMap = std::map <RenderPass_sptr, UniformUpdatingFct>;

RenderProcess(
    const vecRenderPass_sptr& renderPasses,
    PassShaderMap&& shaderPrograms,
    PassUniformUpdateMap&& externalUniformsTreating,
    FrameBuffer_uptr&& frameBuffer);
RenderProcess(const RenderProcess& renderProcess) = delete;
RenderProcess& operator= (const RenderProcess&) = delete;

~RenderProcess() = default;

void updateUniforms();
void render() const;
GLuint getFrameBufferTexture() const;

private:
const vecRenderPass_sptr _renderPasses;
const PassShaderMap _shaderPrograms;
const PassUniformUpdateMap _externalUniformsTreating;
const FrameBuffer_uptr _frameBuffer;
};
