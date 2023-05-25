//
// Created by S.Morgenthaler on 26/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_LEVELPROCESS_H
#define JUMPERBALLAPPLICATION_LEVELPROCESS_H

#include "process/RenderProcess.h"
#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/TextureSampler.h"


class LevelProcess;
using LevelProcess_sptr = std::shared_ptr<LevelProcess>;

class LevelProcess : public RenderProcess {
public:

    static LevelProcess_sptr createInstance(
        const JBTypes::FileContent &fileContent,
        GLsizei width,
        GLsizei height,
        GLuint shadowTexture,
        GLuint shadow2Texture,
        GLsizei shadowsResolution,
        CstRenderGroupsManager_sptr blocks,
        CstRenderGroupsManager_sptr items,
        CstRenderGroupsManager_sptr enemies,
        CstRenderGroupsManager_sptr specials,
        CstRenderGroupsManager_sptr ball,
        CstRenderGroupsManager_sptr star
    );

    LevelProcess(
        GLsizei width,
        GLsizei height,
        ColorableFrameBuffer_uptr frameBuffer,
        GLuint shadowTexture,
        GLuint shadow2Texture,
        std::vector<std::pair<ShaderProgram_sptr, RenderPass_sptr> >&& shadersRenderPasses
    );

    void update() override;

    void render() const override;

    void freeGPUMemory() override;

    vecCstShaderProgram_sptr getShaderPrograms() const override;

    std::shared_ptr<const GLuint> getRenderTexture() const override;

private:

    GLsizei _width;
    GLsizei _height;
    const ColorableFrameBuffer_uptr _frameBuffer;
    const GLuint _shadowTexture;
    const GLuint _shadow2Texture;

    const std::vector<std::pair<ShaderProgram_sptr, RenderPass_sptr> > _shadersRenderPasses;

    static ShaderProgram_sptr createLevelProcessShaderProgram(
        const JBTypes::FileContent &fileContent,
        const std::string &vs,
        GLsizei shadowsResolution
    );
};

#endif //JUMPERBALLAPPLICATION_LEVELPROCESS_H
