//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_BLOOMPROCESS_H
#define JUMPERBALLAPPLICATION_BLOOMPROCESS_H


#include "process/RenderProcess.h"
#include "frameBuffer/TextureSampler.h"

class BloomProcess : public RenderProcess {

public:
    BloomProcess(
        const JBTypes::FileContent &fileContent,
        GLsizei width,
        GLsizei height,
        GLuint sceneHdrTexture,
        GLuint blurTexture,
        GLint defaultFrameBuffer,
        const CstRenderGroupsManager_sptr& screen
    );

    void render() const override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:

    GLsizei _width;
    GLsizei _height;
    RenderPass _screenRenderPass;
    const CstShaderProgram_sptr& _bloomShader;
    TextureSampler _sceneHdrTextureSampler;
    TextureSampler _blurTextureSampler;
    const GLint _defaultFrameBuffer;

    static CstShaderProgram_sptr createBloomProcessShaderProgram(
        const JBTypes::FileContent &fileContent
    );
};


#endif //JUMPERBALLAPPLICATION_BLOOMPROCESS_H
