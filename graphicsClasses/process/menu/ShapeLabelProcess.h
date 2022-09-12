//
// Created by Sébastien Morgenthaler on 28/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_SHAPELABELPROCESS_H
#define JUMPERBALLAPPLICATION_SHAPELABELPROCESS_H


#include "process/RenderProcess.h"
#include "gameMenu/pages/Page.h"

class ShapeLabelProcess: public RenderProcess {
public:
    ShapeLabelProcess(
        const JBTypes::FileContent& fileContent,
        const RenderPass_sptr& renderPassShapes
    );

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;
    vecCstShaderProgram_sptr getShaderPrograms() const override;

private:
    const RenderPass_sptr _renderPassShapes;
    const CstShaderProgram_sptr _shapesShader;
};


#endif //JUMPERBALLAPPLICATION_SHAPELABELPROCESS_H
