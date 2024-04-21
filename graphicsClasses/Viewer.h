/*
 * File: Viewer.h
 * Author: Morgenthaler S
 */

#ifndef VIEWER_H
#define VIEWER_H

#include "RenderingCache.h"
#include "process/menu/LabelsProcess.h"
#include "scene/SceneRendering.h"

class Viewer {
public:
    Viewer(
        GLsizei resolutionX,
        GLsizei resolutionY,
        const CstScene_sptr &scene,
        CstPage_sptr page,
        const JBTypes::FileContent &fileContent,
        const unsigned char* fontData,
        size_t fontDataSize
    );

    ~Viewer();

    void update();

    void resize(unsigned int resolutionX, unsigned int resolutionY);

    void render() const;

    void setPage(const CstPage_sptr &page);

    void setScene(const CstScene_sptr &scene);

private:
    CstScene_sptr _scene;
    CstPage_sptr _page;
    FontTexturesGenerator::FTContent _ftContent;
    const JBTypes::FileContent &_fileContent;

    const GLint _defaultFrameBuffer;

    GLsizei _resolutionX;
    GLsizei _resolutionY;

    RenderingCache _sceneRenderingCache;
    RenderingCache _pageRenderingCache;
    std::unique_ptr<SceneRendering> _sceneRendering;
    std::unique_ptr<LabelsProcess> _pageRendering;

    void resetPageRendering();
    void resetSceneRendering();
    void resetRendering();

};

#endif // VIEWER_H
