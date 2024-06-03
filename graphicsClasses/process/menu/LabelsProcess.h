//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LABELSPROCESS_H
#define JUMPERBALLAPPLICATION_LABELSPROCESS_H

#include "gameMenu/pages/Page.h"
#include "graphicMenu/FontTexturesGenerator.h"
#include "Rendering.h"
#include "scene/Map.h"
#include "process/RenderGroup.h"

class LabelsProcess: Rendering {

public:
    static std::unique_ptr<LabelsProcess> createInstance(
        const JBTypes::FileContent& fileContent,
        const FontTexturesGenerator::FTContent &ftContent,
        GLsizei width,
        GLsizei height,
        const CstPage_sptr &page
    );

    explicit LabelsProcess(
        GLsizei width,
        GLsizei height,
        CstPage_sptr page,
        CstTextureSampler_uptr fontTexture,
        RenderGroup_sptr renderGroup,
        ShaderProgram_sptr labelsShader
    );

    void render() const override;
    void update() override;
    vecCstShaderProgram_sptr getShaderPrograms() const;

private:
    static constexpr GLint characterTextureNumber = 7;
    const CstPage_sptr _page;
    const CstTextureSampler_uptr _fontTexture;
    const RenderGroup_sptr _renderGroup;
    const ShaderProgram_sptr _labelsShader;
    RenderGroupUniforms _renderGroupUniform;
};


#endif //JUMPERBALLAPPLICATION_LABELSPROCESS_H
