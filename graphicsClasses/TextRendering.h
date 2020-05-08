#ifndef TEXTRENDERING_H
#define TEXTRENDERING_H
#include <map>
#include  <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "Utility.h"


class TextRendering
{
public:
    TextRendering(const std::vector<unsigned char>& characters,
                  unsigned int height);
    static bool initFreeTypeAndFont();
    static void clearFreeTypeRessources();

    struct Character {
        GLuint texture;
        glm::ivec2 size;
        glm::ivec2 bearing;
    };

private:

    const std::map<unsigned char, Character> _alphabet;
    static FT_Library ftLib;
    static FT_Face fontFace;

    std::map<unsigned char, Character> initAlphabet(
            const std::vector<unsigned char> characters,  unsigned int height);

};

#endif // TEXTRENDERING_H
