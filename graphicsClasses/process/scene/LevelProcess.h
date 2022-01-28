//
// Created by Sébastien Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LEVELPROCESS_H
#define JUMPERBALLAPPLICATION_LEVELPROCESS_H
#include "process/RenderProcess.h"


class LevelProcess: public RenderProcess {
public:

    LevelProcess(
        GLsizei width,
        GLsizei height,
        const RenderPass_sptr& blocks,
        const RenderPass_sptr& items,
        const RenderPass_sptr& enemies,
        const RenderPass_sptr& specials,
        const RenderPass_sptr& ball,
        const RenderPass_sptr& star
    );

    void render() const override;
    void freeGPUMemory() override;
    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:

    const FrameBuffer_uptr _frameBuffer;

    const RenderPass_sptr _blocks;
    const RenderPass_sptr _items;
    const RenderPass_sptr _enemies;
    const RenderPass_sptr _specials;
    const RenderPass_sptr _ball;
    const RenderPass_sptr _star;

    const CstShaderProgram_sptr _sceneBlocksShader;
    const CstShaderProgram_sptr _sceneItemsShader;
    const CstShaderProgram_sptr _sceneEnemiesShader;
    const CstShaderProgram_sptr _sceneSpecialsShader;
    const CstShaderProgram_sptr _sceneBallShader;
    const CstShaderProgram_sptr _sceneStarShader;

    static const std::string levelFs;
};

#endif //JUMPERBALLAPPLICATION_LEVELPROCESS_H
