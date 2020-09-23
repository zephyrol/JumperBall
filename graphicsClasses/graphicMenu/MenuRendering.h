#ifndef MENURENDERING_H
#define MENURENDERING_H
#include <gameMenu/Menu.h>
#include "PageRendering.h"
#include "TextRendering.h"
#include "BoxRendering.h"

class MenuRendering
{
public:
    MenuRendering(const Menu& menu);

    void render() const;
    void update() ;

private:
    const Menu& _menu;
    const std::map<CstPage_sptr, PageRendering_sptr> _pageRenderings;
    const ShaderProgram _spBox;
    const ShaderProgram _spFont;

    std::map<CstPage_sptr, PageRendering_sptr> createPageRenderings() const;

    static unsigned int getNumberOfPixelsHeight(float height);

    static const std::string vsshaderBox;
    static const std::string fsshaderBox;
    static const std::string vsshaderFont;
    static const std::string fsshaderFont;
};

#endif // MENURENDERING_H
