/*
 * File: FontTexturesGenerator.h
 * Author: Morgenthaler S
 *
 * Created on 22 avril 2021, 10:00
 */

#ifndef FONT_TEXTURES_GENERATOR_H
#define FONT_TEXTURES_GENERATOR_H

#include "Utility.h"

#ifdef __ANDROID__
#include <include/ft2build.h>
#elif defined __APPLE_IOS__
#include <include/ft2build.h>
#else

#include <freetype2/ft2build.h>

#endif

#include FT_FREETYPE_H
#include <gameMenu/Menu.h>

// A font textures generator is attached to a page, because only one page is rendered each time.
class FontTexturesGenerator {

    struct GraphicCharacter {
        GLuint textureID;
        glm::ivec2 size; // Size of the letter
        glm::ivec2 bearing; // Offset from baseline to left/top of the letter
        unsigned int advance; // Offset to advance to next letter
    };

    /**
     * Graphic character key containing the character and its font size in pixels
     */
    using GraphicAlphabet = std::unordered_map<MessageLabel::LetterHash, GraphicCharacter>;


public:

    struct FTContent {
        FT_Library ftLib;
        FT_Face fontFace;
    };

    FontTexturesGenerator(
        size_t screenWidth,
        size_t screenHeight,
        const CstPage_sptr &page,
        const FTContent &ftContent
    );

    void freeGPUMemory();

private:
    const FTContent &_ftContent;

    /**
     * Screen width in pixels
     */
    const size_t _screenWidth;

    /**
     * Screen height in pixels
     */
    const size_t _screenHeight;

    FontTexturesGenerator::GraphicAlphabet _graphicAlphabet;
    const vecMessageLabel_sptr _messageLabels;

    /**
     * Create and add a new graphic character in _graphicAlphabet.
     * @param character Letter character
     * @param pixelHeight Height of the graphic character in pixels
     * @return GraphicCharacter texture added into _graphicAlphabet
     */
    GraphicCharacter createOrGetGraphicCharacter(
        unsigned char character,
        FT_UInt pixelHeight
    );

    static std::vector<MessageLabel::CharacterLocalTransform> getCharacterLocalTransforms(
        const std::vector<GraphicCharacter> &graphicCharacters,
        unsigned int nodePixelWidth,
        unsigned int nodePixelHeight
    );

    vecMessageLabel_sptr genMessageLabels(const CstPage_sptr &page);

public:
    static FTContent initFreeTypeAndFont(
        const unsigned char *fontData,
        size_t fontDataSize
    );

    static void clearFreeTypeRessources(FTContent &ftContent);
};


#endif
