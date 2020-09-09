#ifndef MENURENDERING_H
#define MENURENDERING_H
#include <gameMenu/Menu.h>
#include "TextRendering.h"
#include "BoxRendering.h"

class MenuRendering
{
public:
    MenuRendering(const Menu& menu);

    void render() const;
    void update() ;

private:
    void updatePage(const std::weak_ptr<const Page> &page);
    void renderPage( const std::weak_ptr<const Page>& page) const;

    const Menu& _menu;
    TextRendering _textRendering;
    BoxRendering _boxRendering;

    std::vector<unsigned char> getCharacters
        (const vecCstPage_sptr& pages) const;
    static unsigned int getNumberOfPixelsHeight(float height);
};

#endif // MENURENDERING_H
