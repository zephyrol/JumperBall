#include "MenuRendering.h"

MenuRendering::MenuRendering(const Menu &menu):
    _menu(menu),
    _pageRenderings(createPageRenderings())
{
    update();
}


void MenuRendering::update()
{
    if (_menu.currentPage()){
        _pageRenderings.at(_menu.currentPage())->update();
    }
}

std::map<CstPage_sptr, PageRendering_sptr> MenuRendering::createPageRenderings()
    const
{
    std::map<CstPage_sptr, PageRendering_sptr> pageRenderings;
    for ( const CstPage_sptr& page: _menu.pages()) {
        PageRendering_sptr pageRendering =
                std::make_shared<PageRendering>(*page);
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


