#include "MenuRendering.h"

MenuRendering::MenuRendering(const Menu &menu):
    _menu(menu),
  _textRendering(getCharacters({
    _menu.pausePage(),
    _menu.successPage(),
    _menu.failurePage()}),
    getNumberOfPixelsHeight(menu.rootPage()->height())),
  _boxRendering(glm::vec3(0.f,0.f,1.f),glm::vec3(0.f,1.f,1.f))
{

}

void MenuRendering::updateRendering(const std::weak_ptr<const Page>& page)
{
    CstPage_sptr spPage = page.lock();
    if (spPage->parent().lock() && spPage->visibleOnParent()) {
        updateRendering(spPage->parent());
    }

    for( const CstLabel_sptr& label : page.lock()->labels()) {
        if (label->typeOfLabel() == Label::TypeOfLabel::Message){
            if (label->isActivated()) {
                _textRendering.update(*label, spPage->localPosY());
            } else {
                _textRendering.update(*label, spPage->localPosY());
            }
        } else if (label->typeOfLabel() == Label::TypeOfLabel::Box) {
            _boxRendering.update(*label);
        }
    }

}

void MenuRendering::renderPage( const std::weak_ptr<const Page>& page) const {

    CstPage_sptr spPage = page.lock();
    if (spPage->parent().lock() && spPage->visibleOnParent()) {
        renderPage(spPage->parent());
    }

    for( const CstLabel_sptr& label : page.lock()->labels()) {
        if (label->typeOfLabel() == Label::TypeOfLabel::Message){
            if (label->isActivated()) {
                _textRendering.render(*label, glm::vec3(0, 1.f, 1.f));
            } else {
                _textRendering.render(*label, glm::vec3(.2f, .2f, .2f));
            }
        } else if (label->typeOfLabel() == Label::TypeOfLabel::Box) {
            _boxRendering.render();
            //_boxRendering.render(*label);
        }
    }

}


std::vector<unsigned char> MenuRendering::getCharacters(
        const vecCstPage_sptr& pages) const
{
    std::vector<unsigned char> characters;
 //
    for( const CstPage_sptr& page : pages) {
        for( const CstLabel_sptr& label : page->labels()) {
            if (const CstPage_sptr& childPage = page->child(label)) {

                const std::vector<unsigned char> childCharacters =
                        getCharacters({childPage});
                characters.insert(characters.end(), childCharacters.begin(),
                                  childCharacters.end());
            }
            if (label->typeOfLabel() == Label::TypeOfLabel::Message) {
                for (const char& c : label->message()) {
                    characters.push_back(c);
                }
            }
        }
    }

    return characters;
}

unsigned int MenuRendering::getNumberOfPixelsHeight(float height)
{
    return static_cast<unsigned int> (Utility::windowResolutionY * height);
}


void MenuRendering::render() const
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    renderPage(_menu.currentPage());
}
