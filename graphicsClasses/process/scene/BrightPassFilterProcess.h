//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_BRIGHTPASSFILTERPROCESS_H
#define JUMPERBALLAPPLICATION_BRIGHTPASSFILTERPROCESS_H


#include "process/RenderProcess.h"
#include "frameBuffer/ColorableFrameBuffer.h"

class BrightPassFilterProcess: public RenderProcess {

public:
    BrightPassFilterProcess(
        const JBTypes::FileContent& fileContent,
        GLsizei width,
        GLsizei height,
        GLuint hdrSceneTexture,
        const CstRenderGroupsManager_sptr& screen
    );

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;
    vecCstShaderProgram_sptr getShaderPrograms() const override;
    static CstShaderProgram_sptr createBrightPassFilterProcessShaderProgram(
        const JBTypes::FileContent& fileContent
        ) ;

private:
    const GLsizei _width;
    const GLsizei _height;
    RenderPass _screenRenderPass;
    const CstShaderProgram_sptr& _brightPassFilterShader;
    const ColorableFrameBuffer_uptr _frameBuffer;
    const GLuint _hdrSceneTexture;
};


#endif //JUMPERBALLAPPLICATION_BRIGHTPASSFILTERPROCESS_H
