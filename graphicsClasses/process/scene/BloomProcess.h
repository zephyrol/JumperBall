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
        GLuint blurTexture,
        GLint defaultFrameBuffer,
        const CstRenderGroupsManager_sptr& screen
    );

    void render() const override;

    void freeGPUMemory() override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:

    GLsizei _width;
    GLsizei _height;
    ShaderProgram_sptr _bloomShader;
    RenderPass _screenRenderPass;
    GLuint _blurTexture;
    const GLint _defaultFrameBuffer;

    static ShaderProgram_sptr createBloomProcessShaderProgram(
        const JBTypes::FileContent &fileContent
    );
};


#endif //JUMPERBALLAPPLICATION_BLOOMPROCESS_H
