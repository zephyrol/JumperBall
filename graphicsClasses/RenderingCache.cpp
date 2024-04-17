//
// Created by S.Morgenthaler on 14/04/2024.
//

#include "RenderingCache.h"
RenderingCache::RenderingCache(size_t objectsLifetime)
    : _objectsLifetime(objectsLifetime), _frameBuffers{}, _shaderPrograms{} {}

FrameBuffer_uptr RenderingCache::getFrameBuffer(const std::string& key) {
    return getRenderingObject(key, _frameBuffers);
}

ShaderProgram_uptr RenderingCache::getShaderProgram(const std::string& key) {
    return getRenderingObject(key, _shaderPrograms);
}

void RenderingCache::setFrameBuffer(const std::string& key, FrameBuffer_uptr frameBuffer) {
    CachedRenderingObject<FrameBuffer_uptr> cachedFrameBuffer{std::move(frameBuffer),
                                                                       _objectsLifetime};
    _frameBuffers.emplace(key, std::move(cachedFrameBuffer));
}


void RenderingCache::setShaderProgram(const std::string& key, ShaderProgram_uptr shaderProgram) {
    CachedRenderingObject<ShaderProgram_uptr> cachedShaderProgram{std::move(shaderProgram), _objectsLifetime};
    _shaderPrograms.emplace(key, std::move(cachedShaderProgram));
}

void RenderingCache::decreaseLifetime() {
    decreaseRenderingObjectsLifetime(_frameBuffers);
    decreaseRenderingObjectsLifetime(_shaderPrograms);
}
