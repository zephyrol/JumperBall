#include "MenuRendering.h"

MenuRendering::MenuRendering(const Menu &menu):
    _menu(menu),
    _pageRenderings(createPageRenderings()),
    _spBox( Shader (GL_VERTEX_SHADER, vsshaderBox),
            Shader (GL_FRAGMENT_SHADER, fsshaderBox)),
    _spFont(Shader (GL_VERTEX_SHADER,   vsshaderFont),
            Shader (GL_FRAGMENT_SHADER, fsshaderFont))
{
    update();
}


void MenuRendering::update()
{
    _pageRenderings.at(_menu.currentPage())->update();
}

std::map<CstPage_sptr, PageRendering_sptr> MenuRendering::createPageRenderings()
    const
{
    std::map<CstPage_sptr, PageRendering_sptr> pageRenderings;
    for ( const CstPage_sptr& page: _menu.pages()) {
        PageRendering_sptr pageRendering =
                std::make_shared<PageRendering>(*page,_menu.maxHeight(),
                                                _spFont,_spBox);
        pageRenderings[page]= pageRendering;
    }

    return pageRenderings;
}

void MenuRendering::render() const
{
    if (_menu.currentPage()) {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        _pageRenderings.at(_menu.currentPage())->render();
    }
}

const std::string MenuRendering::vsshaderBox = "shaders/boxVs.vs";
const std::string MenuRendering::fsshaderBox = "shaders/boxFs.fs";
const std::string MenuRendering::vsshaderFont = "shaders/fontVs.vs";
const std::string MenuRendering::fsshaderFont = "shaders/fontFs.fs";

