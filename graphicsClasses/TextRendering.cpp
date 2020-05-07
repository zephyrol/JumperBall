#include "TextRendering.h"

TextRendering::TextRendering()
{

}

bool TextRendering::initFreeTypeAndFont() {

    if (FT_Init_FreeType(&ftLib)) {
        std::cerr << "Error: Impossible to init FreeType Lib..." << std::endl;
        return false;
    }

    if(FT_New_Face(ftLib,"fonts/Cousine-Regular.ttf",
                   0,&fontFace)){
        std::cerr << "Error: Impossible to load the font" <<
                     "Cousine-Regular.ttf ... " << std::endl;
        Utility::displayInstallError();
        FT_Done_FreeType(ftLib);
        return false;
    }
    return true;

}
void TextRendering::clearFreeTypeRessources() {
    FT_Done_Face(fontFace);
    FT_Done_FreeType(ftLib);
}

FT_Library TextRendering::ftLib;
 FT_Face TextRendering::fontFace;
