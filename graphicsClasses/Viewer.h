/*
 * File: Viewer.h
 * Author: Morgenthaler S
 */

#ifndef VIEWER_H
#define VIEWER_H

#include <player/Player.h>
#include <scene/MapGenerator.h>
#include "scene/SceneRendering.h"
#include "graphicMenu/PageRendering.h"

class Viewer {

public:
    Viewer(
        unsigned int resolutionX,
        unsigned int resolutionY,
        const CstScene_sptr &scene,
        const CstPage_sptr &page,
        const JBTypes::FileContent &fileContent,
        const unsigned char *fontData,
        size_t fontDataSize
    );

    ~Viewer();

    void update();

    void resize(unsigned int resolutionX, unsigned int resolutionY);

    void render() const;

    void freeGPUMemory();

    void setSceneRendering(const CstScene_sptr& scene);

    void setPageRendering(const CstPage_sptr& page);

private:

    CstScene_sptr _scene;
    CstPage_sptr _page;
    FontTexturesGenerator::FTContent _ftContent;
    const JBTypes::FileContent &_fileContent;

    const GLint _defaultFrameBuffer;

    GLsizei _resolutionX;
    GLsizei _resolutionY;

    std::unique_ptr<SceneRendering> _sceneRendering;
    std::unique_ptr<PageRendering> _pageRendering;

};

#endif // VIEWER_H
