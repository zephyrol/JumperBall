/*
 * File: Viewer.cpp
 * Author: Morgenthaler S
 */

#include "Viewer.h"

#include <utility>

Viewer::Viewer(
    GLsizei resolutionX,
    GLsizei resolutionY,
    const CstScene_sptr &scene,
    CstPage_sptr page,
    const JBTypes::FileContent &fileContent,
    const unsigned char *fontData,
    size_t fontDataSize
) :
    _scene(scene),
    _page(std::move(page)),
    _ftContent(FontTexturesGenerator::initFreeTypeAndFont(fontData, fontDataSize)),
    _fileContent(fileContent),
    _defaultFrameBuffer([]() {
        GLint defaultFrameBuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFrameBuffer);
        return defaultFrameBuffer;
    }()),
    _resolutionX(static_cast<GLsizei>(resolutionX)),
    _resolutionY(static_cast<GLsizei>(resolutionY)),
    _sceneRenderingCache(2),
    _sceneRendering(SceneRendering::createInstance(*scene,
                                                     _resolutionX,
                                                     _resolutionY,
                                                     _defaultFrameBuffer,
                                                     fileContent,
                                                     _sceneRenderingCache)),
    _pageRendering(LabelsProcess::createInstance(
        fileContent,
        _ftContent,
        _resolutionX,
        _resolutionY,
        _page
    )) {
}

void Viewer::update() {
    _sceneRendering->update();
    _pageRendering->update();
}

void Viewer::render() const {
    _sceneRendering->render();
    _pageRendering->render();
}

void Viewer::resize(unsigned int resolutionX, unsigned int resolutionY) {
    _resolutionX = static_cast<GLsizei>(resolutionX);
    _resolutionY = static_cast<GLsizei>(resolutionY);
    resetRendering();
}

void Viewer::setScene(const CstScene_sptr &scene) {
    _scene = scene;
    resetSceneRendering();
    resetPageRendering();
}

void Viewer::setPage(const CstPage_sptr &page) {
    _page = page;
    resetPageRendering();
}

Viewer::~Viewer() {
    FontTexturesGenerator::clearFreeTypeResources(_ftContent);
}

void Viewer::resetRendering() {
    resetSceneRendering();
    resetPageRendering();
}

void Viewer::resetPageRendering() {
    // Free memory first
    _pageRendering = nullptr;
    _pageRendering = std::unique_ptr<LabelsProcess>(LabelsProcess::createInstance(
        _fileContent,
        _ftContent,
        _resolutionX,
        _resolutionY,
        _page
    ));
}

void Viewer::resetSceneRendering() {
    _sceneRendering->fillCache(_sceneRenderingCache);
    // Free memory
    //_sceneRendering = nullptr;
    _sceneRendering = SceneRendering::createInstance(*_scene, _resolutionX, _resolutionY, _defaultFrameBuffer,
                                                     _fileContent, _sceneRenderingCache);
    _sceneRenderingCache.decreaseLifetime();
}

