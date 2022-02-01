//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_BLOOMPROCESS_H
#define JUMPERBALLAPPLICATION_BLOOMPROCESS_H


#include "process/RenderProcess.h"

class BloomProcess: public RenderProcess {

public:
    BloomProcess(
        GLsizei width,
        GLsizei height,
        GLuint sceneHDRTexture,
        GLuint bluredTexture,
        const RenderPass_sptr& screen
    );

    void render() const override;
    void freeGPUMemory() override;
    vecCstShaderProgram_sptr getShaderPrograms() const override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:

    GLsizei _width;
    GLsizei _height;
    const RenderPass_sptr _screen;
    const GLuint _sceneHDRTexture;
    const GLuint _bluredTexture;
    const CstShaderProgram_sptr _bloomShader;

    static CstShaderProgram_sptr createBloomProcessShaderProgram() ;
};


#endif //JUMPERBALLAPPLICATION_BLOOMPROCESS_H
