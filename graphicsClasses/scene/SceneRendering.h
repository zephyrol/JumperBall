/*
 * File: SceneRendering.h
 * Author: Morgenthaler S
 *
 * Created on 6 novembre 2019, 20:38
 */

#ifndef SCENE_RENDERING_H
#define SCENE_RENDERING_H

#include "Rendering.h"
#include "RenderingCache.h"
#include "Scene.h"
#include "process/RenderProcess.h"
#include "process/UniformBuffer.h"
#include "process/scene/LevelProcess.h"
#include "process/scene/PostEffects.h"

class SceneRendering : public Rendering {
   public:
    SceneRendering(const Scene& scene,
                   GLsizei width,
                   GLsizei height,
                   LevelProcess_uptr levelProcess,
                   PostEffects_uptr postEffects,
                   UniformBuffer_uptr sceneUniformBuffer);

    static std::unique_ptr<SceneRendering> createInstance(const Scene& scene,
                                                          GLsizei width,
                                                          GLsizei height,
                                                          GLint defaultFrameBuffer,
                                                          const JBTypes::FileContent& fileContent,
                                                          RenderingCache& renderingCache);

    void update() override;

    void render() const override;

    void fillCache(RenderingCache& renderingCache);

   private:
    const Scene& _scene;
    LevelProcess_uptr _levelProcess;
    PostEffects_uptr _postEffects;
    UniformBuffer_uptr _sceneUniformBuffer;
};

#endif /* SCENE_RENDERING_H */
