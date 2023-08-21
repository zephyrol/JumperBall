//
// Created by S.Morgenthaler on 21/08/2023.
//

#include "PostProcesses.h"

PostProcesses::PostProcesses(
    const JBTypes::FileContent &fileContent,
    GLsizei width,
    GLsizei height,
    GLuint sceneTexture,
    GLint defaultFrameBuffer,
    const CstRenderGroup_sptr &screen
) :
    _width(width),
    _height(height),
    _postProcessesShader(createPostProcessesShaderProgram(fileContent, width, height)),
    _screenRenderPass(_postProcessesShader, screen),
    _frameBufferA(ColorableFrameBuffer::createInstance(
        width,
        height,
        true,
        false
    )),
    _frameBufferB(ColorableFrameBuffer::createInstance(
        width,
        height,
        false,
        false
    )),
    _sceneTexture(sceneTexture),
    _defaultFrameBuffer(defaultFrameBuffer) {
}
