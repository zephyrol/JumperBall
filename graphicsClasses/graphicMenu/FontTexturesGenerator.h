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
        // Texture info
        glm::ivec2 pixelsUvPixelsMin;
        glm::ivec2 pixelsUvPixelsMax;

        // Letter info
        glm::ivec2 size; // Size of the letter
        glm::ivec2 bearing; // Offset from baseline to left/top of the letter
        unsigned int advance; // Offset to advance to next letter
    };

    /**
     * Graphic character key containing the character and its font size in pixels
     */
    using GraphicAlphabet = std::unordered_map<MessageLabel::LetterHash, GraphicCharacter>;

    /**
     * Struct containing letters texture state
     */
    struct LettersTexture {

        /**
         * Current texture data
         */
        std::vector<unsigned char> lettersData {};

        /**
         * Current texture width
         */
        unsigned int width = 0;

        /**
         * Current texture height
         */
        unsigned int height = 0;

        /**
         * Current cursor where the next letter should be written
         */
        glm::ivec2 cursor = glm::ivec2(0, 0);

        /**
         * Graphic alphabet
         */
         GraphicAlphabet graphicAlphabet {};

         /**
          * Texture Id
          */
          GLuint textureID = 0;
    };

public:

    struct FTContent {
        FT_Library ftLib;
        FT_Face fontFace;
    };

    FontTexturesGenerator(
        size_t screenWidth,
        size_t screenHeight,
        const CstPage_sptr &page,
        const FontTexturesGenerator::FTContent &ftContent,
        const LettersTexture &lettersTexture,
        vecMessageLabel_sptr&& messageLabels
    );

    FontTexturesGenerator(const FontTexturesGenerator& ft) = default;
    FontTexturesGenerator& operator = (const FontTexturesGenerator& ft) = delete;

    static FontTexturesGenerator createInstance(
        size_t screenWidth,
        size_t screenHeight,
        const CstPage_sptr &page,
        const FTContent &ftContent
    );

    void freeGPUMemory();

    const vecMessageLabel_sptr &getMessageLabels();

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

    const LettersTexture _lettersTexture;

    const vecMessageLabel_sptr _messageLabels;


    /**
     * Create and add a new graphic character in alphabet.
     * @param character Letter character
     * @param pixelHeight Height of the graphic character in pixels
     * @param lettersTexture Current alphabet reference, if the character width pixelHeight height
     * is not added yet, currentAlphabet is directly modified.
     * @return GraphicCharacter texture added into currentAlphabet param
     */
    static FontTexturesGenerator::GraphicCharacter createOrGetGraphicCharacter(size_t screenWidth,
                                                                               const FontTexturesGenerator::FTContent &ftContent,
                                                                               FT_UInt pixelHeight,
                                                                               LettersTexture &lettersTexture,
                                                                               unsigned char character);

    static std::vector<MessageLabel::CharacterLocalTransform> getCharacterLocalTransforms(
        const std::vector<GraphicCharacter> &graphicCharacters,
        unsigned int nodePixelWidth,
        unsigned int nodePixelHeight
    );

    static glm::ivec4 insertCharacterToTexture(
        size_t screenWidth,
        FontTexturesGenerator::LettersTexture &lettersTexture,
        const unsigned char *letterBitmap,
        unsigned int bitmapWidth,
        unsigned int bitmapHeight
    );

public:
    static FTContent initFreeTypeAndFont(
        const unsigned char *fontData,
        size_t fontDataSize
    );

    static void clearFreeTypeRessources(FTContent &ftContent);
};


#endif
