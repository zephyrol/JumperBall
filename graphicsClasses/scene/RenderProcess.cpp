/*
 * File: RenderProcess.cpp
 * Author: Morgenthaler S
 *
 * Created on 25 mars 2021, 21:48
 */
#include "RenderProcess.h"

RenderProcess::RenderProcess(
  ShaderProgram&& shaderProgram,
  FrameBuffer&& frameBuffer,
  const vecRenderPass_sptr& renderPasses) :
  _shaderProgram(std::move(shaderProgram)),
  _frameBuffer(std::move(frameBuffer)),
  _renderPasses(renderPasses)
{
  
}
