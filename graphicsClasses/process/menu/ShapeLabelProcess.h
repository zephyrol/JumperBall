//
// Created by Sébastien Morgenthaler on 28/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_SHAPELABELPROCESS_H
#define JUMPERBALLAPPLICATION_SHAPELABELPROCESS_H


#include "process/RenderProcess.h"
#include "gameMenu/Page.h"

class ShapeLabelProcess: RenderProcess {
public:
    ShapeLabelProcess(
        const RenderPass_sptr& renderPass
    );

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    const RenderPass_sptr _renderPassesShapes;
    const CstShaderProgram_sptr _shapesShader;
};


#endif //JUMPERBALLAPPLICATION_SHAPELABELPROCESS_H
