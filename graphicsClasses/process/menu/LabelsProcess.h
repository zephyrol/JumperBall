//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LABELSPROCESS_H
#define JUMPERBALLAPPLICATION_LABELSPROCESS_H


#include "process/RenderProcess.h"
#include "gameMenu/pages/Page.h"
#include "graphicMenu/FontTexturesGenerator.h"
#include "Rendering.h"
#include "frameBuffer/TextureSampler.h"

class LabelsProcess: public RenderProcess, Rendering {

public:
    static std::unique_ptr<LabelsProcess> createInstance(
        const JBTypes::FileContent& fileContent,
        const FontTexturesGenerator::FTContent &ftContent,
        GLsizei width,
        GLsizei height,
        const CstPage_sptr &page,
        CstMap_sptr map
    );

    explicit LabelsProcess(
        GLsizei width,
        GLsizei height,
        CstPage_sptr page,
        const FontTexturesGenerator& fontTexturesGenerator,
        TextureSampler textureSampler,
        RenderGroupsManager_sptr renderGroupsManager,
        RenderPass renderPass,
        CstShaderProgram_sptr labelsShader,
        CstMap_sptr map
    );

    void render() const override;
    void update() override;
    void freeGPUMemory() override;
    vecCstShaderProgram_sptr getShaderPrograms() const override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:
    const CstPage_sptr _page;
    FontTexturesGenerator _fontTexturesGenerator;
    TextureSampler _characterTextureSampler;
    RenderGroupsManager_sptr _renderGroupsManager;
    RenderPass _renderPass;
    const CstShaderProgram_sptr _labelsShader;
    CstMap_sptr _map;
};


#endif //JUMPERBALLAPPLICATION_LABELSPROCESS_H
