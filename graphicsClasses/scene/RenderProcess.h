/*
 * File: RenderProcess.h
 * Author: Morgenthaler S
 *
 * Created on 25 mars 2021, 21:48
 */

#include "RenderPass.h"
#include "FrameBuffer.h"
#include <functional>

class RenderProcess;
using RenderProcess_sptr = std::shared_ptr <RenderProcess>;
using CstRenderProcess_sptr = std::shared_ptr <const RenderProcess>;
using vecCstRenderProcess_sptr = std::vector <CstRenderProcess_sptr>;
using vecRenderProcess_sptr = std::vector <RenderProcess_sptr>;

class RenderProcess {

public:

using UniformUpdatingFct = std::function <void (const RenderPass_sptr&, GLuint)>;
using PassShaderMap = std::unordered_map <RenderPass_sptr, CstShaderProgram_sptr>;
using PassUniformUpdateMap = std::unordered_map <RenderPass_sptr, UniformUpdatingFct>;

RenderProcess(
    const vecRenderPass_sptr& renderPasses,
    PassShaderMap&& shaderPrograms,
    PassUniformUpdateMap&& externalUniformsTreating,
    FrameBuffer_uptr&& frameBuffer,
    bool usingDefaultFrameBufferAutoClean = true
);

RenderProcess(
    const vecRenderPass_sptr& renderPasses,
    PassShaderMap&& shaderPrograms,
    PassUniformUpdateMap&& externalUniformsTreating,
    GLsizei defaultFrameBufferWidth,
    GLsizei defaultFrameBufferHeight,
    bool usingDefaultFrameBufferAutoClean = true
);

RenderProcess(const RenderProcess& renderProcess) = delete;
RenderProcess& operator= (const RenderProcess&) = delete;

~RenderProcess() = default;

void updateUniforms();
void render() const;
void freeGPUMemory();
GLuint getFrameBufferTexture() const;

private:
const vecRenderPass_sptr _renderPasses;
const PassShaderMap _shaderPrograms;
const PassUniformUpdateMap _externalUniformsTreating;
const FrameBuffer_uptr _frameBuffer;
const std::unique_ptr<const GLsizei> _defaultFrameBufferWidth;
const std::unique_ptr<const GLsizei> _defaultFrameBufferHeight;
const bool _usingDefaultFrameBufferAutoClean;
};
