//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LETTERSPROCESS_H
#define JUMPERBALLAPPLICATION_BLOOMPROCESS_H


#include "process/RenderProcess.h"

class BloomProcess: RenderProcess {

public:
    BloomProcess(
        GLuint sceneHDRTexture,
        GLuint bluredTexture,
        const RenderPass_sptr& screen);

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    const RenderPass_sptr _screen;
    const GLuint _sceneHDRTexture;
    const GLuint _bluredTexture;
    const CstShaderProgram_sptr _bloomShader;

    CstShaderProgram_sptr createBloomProcessShaderProgram() const;
};


#endif //JUMPERBALLAPPLICATION_LETTERSPROCESS_H
