/*
 * File: Viewer.cpp
 * Author: Morgenthaler S
 */

#include "Viewer.h"

Viewer::Viewer(
    unsigned int resolutionX,
    unsigned int resolutionY,
    const CstScene_sptr &scene,
    const CstPage_sptr &page,
    const JBTypes::FileContent &fileContent,
    const unsigned char *fontData,
    size_t fontDataSize
) :
    _scene(scene),
    _page(page),
    _ftContent(FontTexturesGenerator::initFreeTypeAndFont(fontData, fontDataSize)),
    _fileContent(fileContent),
    _defaultFrameBuffer([]() {
        GLint defaultFrameBuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFrameBuffer);
        return defaultFrameBuffer;
    }()),
    _resolutionX(static_cast<GLsizei>(resolutionX)),
    _resolutionY(static_cast<GLsizei>(resolutionY)),
    _sceneRendering(new SceneRendering(
        *scene,
        _resolutionX,
        _resolutionY,
        _defaultFrameBuffer,
        fileContent
    )),
    _pageRendering(new LabelsProcess(
        fileContent,
        _ftContent,
        _resolutionX,
        _resolutionY,
        _page,
        scene->getMap()
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

void Viewer::freeGPUMemory() {
    _sceneRendering->freeGPUMemory();
    _pageRendering->freeGPUMemory();
}

void Viewer::resize(unsigned int resolutionX, unsigned int resolutionY) {
    _resolutionX = static_cast<GLsizei>(resolutionX);
    _resolutionY = static_cast<GLsizei>(resolutionY);
    resetSceneRendering();
    resetPageRendering();
}

void Viewer::setScene(const CstScene_sptr &scene) {
    _scene = scene;
    resetSceneRendering();
}

void Viewer::setPage(const CstPage_sptr &page) {
    _page = page;
    resetPageRendering();
}

Viewer::~Viewer() {
    FontTexturesGenerator::clearFreeTypeRessources(_ftContent);
}

void Viewer::resetPageRendering() {
    _pageRendering->freeGPUMemory();
    _pageRendering = std::unique_ptr<LabelsProcess>(new LabelsProcess(
        _fileContent,
        _ftContent,
        _resolutionX,
        _resolutionY,
        _page,
        _scene->getMap()
    ));
}

void Viewer::resetSceneRendering() {
    _sceneRendering->freeGPUMemory();
    _sceneRendering = std::unique_ptr<SceneRendering>(new SceneRendering(
        *_scene,
        _resolutionX,
        _resolutionY,
        _defaultFrameBuffer,
        _fileContent
    ));
    _pageRendering->setMap(_scene->getMap());
}
