/*
 * File: SceneRendering.h
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#ifndef SCENE_RENDERING_H
#define SCENE_RENDERING_H

#include "componentsGeneration/RenderGroupGenerator.h"
#include "Rendering.h"
#include "Scene.h"
#include "process/scene/SceneUniformBuffer.h"
#include "process/RenderProcess.h"


class SceneRendering : public Rendering {

public:

    SceneRendering(
        const Scene &scene,
        GLsizei width,
        GLsizei height,
        vecRenderProcess_sptr&& processes,
        SceneUniformBuffer&& sceneUniformBuffer
    );

    static std::unique_ptr<SceneRendering> createInstance(
        const Scene &scene,
        GLsizei width,
        GLsizei height,
        GLint defaultFrameBuffer,
        const JBTypes::FileContent &fileContent
    );

    void update() override;

    void freeGPUMemory() override;

    void render() const override;

private:

    const Scene &_scene;
    const vecRenderProcess_sptr _processes;
    SceneUniformBuffer _sceneUniformBuffer;
};

#endif /* SCENE_RENDERING_H */
