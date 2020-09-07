#ifndef MENURENDERING_H
#define MENURENDERING_H
#include "TextRendering.h"
#include "BoxRendering.h"
#include "Menu.h"

class MenuRendering
{
public:
    MenuRendering(const Menu& menu);

    void render() const;
    void updateRendering(const std::weak_ptr<const Page> &page);
    void renderPage( const std::weak_ptr<const Page>& page) const;

    const Menu& _menu;
    TextRendering _textRendering;
    BoxRendering _boxRendering;

    std::vector<unsigned char> getCharacters
        (const vecCstPage_sptr& pages) const;
    static unsigned int getNumberOfPixelsHeight(float height);
};

#endif // MENURENDERING_H
