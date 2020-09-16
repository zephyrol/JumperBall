#include "MenuRendering.h"

MenuRendering::MenuRendering(const Menu &menu):
    _menu(menu),
    _pageRenderings(createPageRenderings())
{
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
        std::cout << "nombre de pages :"  << _menu.pages().size() << std::endl;
        PageRendering_sptr pageRendering =
                std::make_shared<PageRendering>(*page,_menu.maxHeight());
        pageRenderings[page]= pageRendering;
    }

    return pageRenderings;
}

void MenuRendering::render() const
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    _pageRenderings.at(_menu.currentPage())->render();

}
