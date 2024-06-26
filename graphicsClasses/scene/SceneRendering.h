/*
 * File: SceneRendering.h
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#ifndef SCENE_RENDERING_H
#define SCENE_RENDERING_H

#include "Rendering.h"
#include "Scene.h"
#include "process/RenderProcess.h"
#include "process/scene/LevelProcess.h"
#include "process/scene/PostEffects.h"
#include "process/scene/SceneUniformBuffer.h"

class SceneRendering : public Rendering {
   public:
    SceneRendering(const Scene& scene,
                   GLsizei width,
                   GLsizei height,
                   LevelProcess_uptr levelProcess,
                   PostEffects_uptr postEffects,
                   SceneUniformBuffer&& sceneUniformBuffer);

    static std::unique_ptr<SceneRendering> createInstance(const Scene& scene,
                                                          GLsizei width,
                                                          GLsizei height,
                                                          GLint defaultFrameBuffer,
                                                          const JBTypes::FileContent& fileContent);

    void update() override;

    void render() const override;

   private:
    static const glm::mat4 shadowBiasMatrix;

    const Scene& _scene;
    const LevelProcess_uptr _levelProcess;
    const PostEffects_uptr _postEffects;
    SceneUniformBuffer _sceneUniformBuffer;
};

#endif /* SCENE_RENDERING_H */
