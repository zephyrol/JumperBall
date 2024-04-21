//
// Created by S.Morgenthaler on 14/04/2024.
//

#ifndef RENDERINGCACHE_H
#define RENDERINGCACHE_H
#include "frameBuffer/ColorableFrameBuffer.h"
#include "frameBuffer/DepthFrameBuffer.h"

class RenderingCache {
   public:
    /**
     * Create a rendering cache.
     * @param objectsLifetime Lifetime that is set to each object when they are inserted into the cache.
     */
    explicit RenderingCache(size_t objectsLifetime);

    /**
     * Get a frame buffer from the cache. The caller becomes the owner.
     * @param key Key as string.
     * @return The related frame buffer if it exists, otherwise nullptr.
     */
    FrameBuffer_uptr getFrameBuffer(const std::string& key);

    /**
     * Get a shader program from the cache. The caller becomes the owner.
     * @param key Key as string.
     * @return The related shader program if it exists, otherwise nullptr.
     */
    ShaderProgram_uptr getShaderProgram(const std::string& key);

    /**
     * Get a texture from the cache. The caller becomes the owner.
     * @param key Key as string.
     * @return The related texture if it exists, otherwise nullptr.
     */
    CstTextureSampler_uptr getTexture(const std::string& key);

    /**
     * Set a frame buffer in the cache. The cache becomes the owner.
     */
    void setFrameBuffer(const std::string& key, FrameBuffer_uptr frameBuffer);

    /**
     * Set a shader program in the cache. The cache becomes the owner.
     */
    void setShaderProgram(const std::string& key, ShaderProgram_uptr shaderProgram);

    /**
     * Set a texture in the cache. The cache becomes the owner.
     */
    void setTexture(const std::string& key, CstTextureSampler_uptr texture);

    /**
     * Decrease the lifetime of all the rendering objects existing in the cache. For each rendering object,
     * if the lifetime is equal to 0, the object is destroyed.
     */
    void decreaseLifetime();

   private:
    const size_t _objectsLifetime;

    template <typename T>
    struct CachedRenderingObject {
        T renderingObjectPtr;
        size_t currentLifetime;
    };

    template <typename T>
    void decreaseRenderingObjectsLifetime(
        std::unordered_map<std::string, CachedRenderingObject<T>>& renderingObjects);

    template <typename T>
    static T getRenderingObject(const std::string& key,
                                std::unordered_map<std::string, CachedRenderingObject<T>>& renderingObjects);

    std::unordered_map<std::string, CachedRenderingObject<FrameBuffer_uptr>> _frameBuffers;
    std::unordered_map<std::string, CachedRenderingObject<ShaderProgram_uptr>> _shaderPrograms;
    std::unordered_map<std::string, CachedRenderingObject<CstTextureSampler_uptr>> _textures;
};

template <typename T>
void RenderingCache::decreaseRenderingObjectsLifetime(
    std::unordered_map<std::string, CachedRenderingObject<T>>& renderingObjects) {
    std::vector<std::string> renderingObjectsToDeleteKeys{};

    for (auto& keyRenderingObject : renderingObjects) {
        auto& cachedRenderingObject = keyRenderingObject.second;
        --cachedRenderingObject.currentLifetime;
        if (cachedRenderingObject.currentLifetime == 0) {
            const auto& key = keyRenderingObject.first;
            renderingObjectsToDeleteKeys.emplace_back(key);
        }
    }

    for (const auto& key : renderingObjectsToDeleteKeys) {
        renderingObjects.erase(key);
    }
}
template <typename T>
T RenderingCache::getRenderingObject(
    const std::string& key,
    std::unordered_map<std::string, CachedRenderingObject<T>>& renderingObjects) {
    const auto cachedRenderingObjectIterator = renderingObjects.find(key);
    if (cachedRenderingObjectIterator == renderingObjects.end()) {
        return nullptr;
    }

    T renderingObject = std::move(cachedRenderingObjectIterator->second.renderingObjectPtr);
    renderingObjects.erase(cachedRenderingObjectIterator);
    return renderingObject;
}

#endif  // RENDERINGCACHE_H
