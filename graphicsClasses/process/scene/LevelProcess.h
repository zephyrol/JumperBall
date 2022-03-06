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
        const RenderPass_sptr& blocks,
        const RenderPass_sptr& items,
        const RenderPass_sptr& enemies,
        const RenderPass_sptr& specials,
        const RenderPass_sptr& ball,
        const RenderPass_sptr& star
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
    const CstShaderProgram_sptr _sceneBlocksShader;
    const CstShaderProgram_sptr _sceneItemsShader;
    const CstShaderProgram_sptr _sceneEnemiesShader;
    const CstShaderProgram_sptr _sceneSpecialsShader;
    const CstShaderProgram_sptr _sceneBallShader;
    const CstShaderProgram_sptr _sceneStarShader;

    const std::map<CstShaderProgram_sptr, RenderPass_sptr> _shadersRenderPasses;

    static CstShaderProgram_sptr createLevelProcessShaderProgram(
        const JBTypes::FileContent& fileContent,
        const std::string& vs
    ) ;

    static const std::string levelFs;
    static const std::vector<std::string> levelDefines;
};

#endif //JUMPERBALLAPPLICATION_LEVELPROCESS_H
