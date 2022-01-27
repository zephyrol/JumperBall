//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_BLOOMPROCESS_H
#define JUMPERBALLAPPLICATION_BLOOMPROCESS_H


#include "RenderProcess.h"

class BloomProcess: RenderProcess {

public:
    BloomProcess(GLsizei width, GLsizei height, const RenderPass_sptr& screen);

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    const GLsizei _width;
    const GLsizei _height;
    const RenderPass_sptr _screen;
    const CstShaderProgram_sptr _bloomBlurShader;
};


#endif //JUMPERBALLAPPLICATION_BLOOMPROCESS_H
