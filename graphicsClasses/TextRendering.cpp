#include "TextRendering.h"

TextRendering::TextRendering(const std::vector<unsigned char>& characters,
                             unsigned int height):
    _alphabet(initAlphabet(characters,height))
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

std::map<unsigned char, TextRendering::Character> TextRendering::initAlphabet(
        const std::vector<unsigned char> characters, unsigned int height)
{
    std::map<unsigned char, TextRendering::Character> alphabet;

    FT_Set_Pixel_Sizes(fontFace,0,height);
    for (const unsigned char& character : characters) {

        if (FT_Load_Char(fontFace,character,FT_LOAD_RENDER)) {

            GLuint textureID;
            glGenTextures(1,&textureID);
            glBindTexture(GL_TEXTURE_2D,textureID);

            constexpr GLint levelOfDetail = 0;
            glTexImage2D(GL_TEXTURE_2D, levelOfDetail, GL_RED,
                         fontFace->glyph->bitmap.width,
                         fontFace->glyph->bitmap.rows, 0, GL_RED,
                         GL_UNSIGNED_BYTE,
                         fontFace->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            const TextRendering::Character graphicChar {
                textureID,
                {fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows},
                {fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top}
            };
            alphabet[character] = graphicChar;
        }
        else {
            std::cerr << "Error... Impossible to load glyph " << character <<
                         std::endl;
        }
    }
    return alphabet;
}


FT_Library TextRendering::ftLib;
FT_Face TextRendering::fontFace;
