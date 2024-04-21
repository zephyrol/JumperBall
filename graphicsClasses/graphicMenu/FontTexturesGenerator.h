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

#include "RenderingCache.h"
#include "frameBuffer/TextureSampler.h"

class FontTexturesGenerator;
using FontTexturesGenerator_uptr = std::unique_ptr<FontTexturesGenerator>;

/**
 * A font textures generator is attached to a page, because only one page is rendered on the same time.
 */
class FontTexturesGenerator {
    struct GraphicCharacter {
        // Texture info
        glm::ivec2 pixelsUvPixelsMin;
        glm::ivec2 pixelsUvPixelsMax;

        // Letter info
        glm::ivec2 size;       // Size of the letter
        glm::ivec2 bearing;    // Offset from baseline to left/top of the letter
        unsigned int advance;  // Offset to advance to next letter
    };

    /**
     * Graphic character key containing the character and its font size in pixels
     */
    using GraphicAlphabet = std::unordered_map<TextLabel::LetterHash, GraphicCharacter>;

    /**
     * Struct containing letters texture state
     */
    struct LettersTexture {
        /**
         * Current texture data
         */
        std::vector<unsigned char> lettersData{};

        /**
         * Texture width
         */
        unsigned int width = 0;

        /**
         * Texture height
         */
        unsigned int height = 0;

        /**
         * Current cursor where the next letter should be written
         */
        glm::ivec2 cursor = glm::ivec2(0, 0);

        /**
         * Current max letter height in the current line in pixels
         */
        unsigned int currentMaxLetterHeight = 0;

        /**
         * Graphic alphabet
         */
        GraphicAlphabet graphicAlphabet{};

        /**
         * Texture Id
         */
        CstTextureSampler_uptr texture = nullptr;
    };

   public:
    struct FTContent {
        FT_Library ftLib;
        FT_Face fontFace;
    };

    FontTexturesGenerator(LettersTexture lettersTexture, vecTextLabel_sptr&& textLabels);

    FontTexturesGenerator& operator=(const FontTexturesGenerator& ft) = delete;

    static FontTexturesGenerator_uptr createInstance(size_t screenWidth,
                                                     size_t screenHeight,
                                                     const CstPage_sptr& page,
                                                     const FTContent& ftContent);

    const CstTextureSampler_uptr& getLettersTexture() const;

    const vecTextLabel_sptr& getTextLabels();

    void fillCache(RenderingCache& renderingCache);

    static FTContent initFreeTypeAndFont(const unsigned char* fontData, size_t fontDataSize);

    static void clearFreeTypeResources(FTContent& ftContent);

    static const std::string lettersTextureHash;

   private:
    const LettersTexture _lettersTexture;

    const vecTextLabel_sptr _messageLabels;

    /**
     * Create and add a new graphic character in alphabet.
     * @param character Letter character
     * @param pixelHeight Height of the graphic character in pixels
     * @param lettersTexture Current alphabet reference, if the character width pixelHeight height
     * is not added yet, currentAlphabet is directly modified.
     * @return GraphicCharacter texture added into currentAlphabet param
     */
    static FontTexturesGenerator::GraphicCharacter createOrGetGraphicCharacter(
        const FontTexturesGenerator::FTContent& ftContent,
        FT_UInt pixelHeight,
        LettersTexture& lettersTexture,
        unsigned char character);

    static std::vector<TextLabel::CharacterLocalTransform> getCharacterLocalTransforms(
        const std::vector<GraphicCharacter>& graphicCharacters,
        unsigned int nodePixelWidth,
        unsigned int nodePixelHeight);

    /**
     * Insert bitmap character into letters texture.
     * @param lettersTexture Current letters texture reference. This is directly modified (side effect).
     * @param letterBitmap Buffer containing letter.
     * @param bitmapWidth Width of the letter in pixels.
     * @param bitmapHeight Height of the letter in pixels.
     * @return vec4 containing uvMinX, uvMinY, uvMaxX, uvMaxY of the letter in the texture (in pixels).
     */
    static glm::ivec4 insertCharacterToTexture(FontTexturesGenerator::LettersTexture& lettersTexture,
                                               const unsigned char* letterBitmap,
                                               unsigned int bitmapWidth,
                                               unsigned int bitmapHeight);
};

#endif
