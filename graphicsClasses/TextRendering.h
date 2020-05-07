#ifndef TEXTRENDERING_H
#define TEXTRENDERING_H
#include  <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "Utility.h"

class TextRendering
{
public:
    TextRendering();
    static bool initFreeTypeAndFont();
    static void clearFreeTypeRessources();

private:

    static FT_Library ftLib;
    static FT_Face fontFace;

};

#endif // TEXTRENDERING_H
