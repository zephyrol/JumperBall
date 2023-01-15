//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LEVELPROCESS_H
#define JUMPERBALLAPPLICATION_LEVELPROCESS_H
#include "process/RenderProcess.h"


class LevelProcess: public RenderProcess {
public:

    LevelProcess(
        const JBTypes::FileContent& fileContent,
        GLsizei width,
        GLsizei height,
        GLuint shadowTexture,
        GLuint shadow2Texture,
        RenderPass_sptr  blocks,
        RenderPass_sptr  items,
        RenderPass_sptr  enemies,
        RenderPass_sptr  specials,
        RenderPass_sptr  ball,
        RenderPass_sptr  star
    );

    void render() const override;
    void freeGPUMemory() override;
    vecCstShaderProgram_sptr getShaderPrograms() const override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:

    GLsizei _width;
    GLsizei _height;
    const FrameBuffer_uptr _frameBuffer;

    const RenderPass_sptr _blocks;
    const RenderPass_sptr _items;
    const RenderPass_sptr _enemies;
    const RenderPass_sptr _specials;
    const RenderPass_sptr _ball;
    const RenderPass_sptr _star;

    const GLuint _shadowTexture;
    const GLuint _shadow2Texture;
    const CstShaderProgram_sptr _sceneBlocksShader;
    const CstShaderProgram_sptr _sceneItemsShader;
    const CstShaderProgram_sptr _sceneEnemiesShader;
    const CstShaderProgram_sptr _sceneSpecialsShader;
    const CstShaderProgram_sptr _sceneBallShader;
    const CstShaderProgram_sptr _sceneStarShader;

    const std::vector<std::pair<CstShaderProgram_sptr, RenderPass_sptr> > _shadersRenderPasses;

    static CstShaderProgram_sptr
    createLevelProcessShaderProgram(
        const JBTypes::FileContent &fileContent,
        const std::string &vs,
        std::vector<std::string>&& uniformNames
    );
};

#endif //JUMPERBALLAPPLICATION_LEVELPROCESS_H
