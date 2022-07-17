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

class FontTexturesGenerator {

    struct CharacterLocalTransform {
        /**
         * Size of the letter in pixels
         */
        glm::ivec2 size;

        /**
         * Offset from baseline to left/top of the letter in pixels
         */
        glm::ivec2 bearing;

        /**
         * Offset until the next letter (in 1/64 pixels)
         */
        FT_Pos advance;
    };

    struct GraphicCharacter {
        GLuint textureID;
        CharacterLocalTransform transform;
    };

    struct FTContent {
        FT_Library ftLib;
        FT_Face fontFace;
    };

    //using GraphicAlphabet = std::unordered_map<unsigned char, GraphicCharacter>;

    /**
     * Graphic character key containing the character and its font size in pixels
     */
    using GraphicCharacterHash = std::string;
    using GraphicAlphabet = std::unordered_map<GraphicCharacterHash, GraphicCharacter>>;

    using NodeMessageAssociations = std::unordered_map<CstNode_sptr, std::string>;

    vecMessageLabel_sptr genMessageLabels(const NodeMessageAssociations& nodeToMessage);

public:
    FontTexturesGenerator(
        size_t screenHeight,
        const FTContent &ftContent,
        const NodeMessageAssociations &nodeToMessage
    );

private:
    const FTContent &_ftContent;

    /**
     * Screen height in pixels
     */
    const size_t _screenHeight;

    FontTexturesGenerator::GraphicAlphabet _graphicAlphabet;
    const vecMessageLabel_sptr _messageLabels;

    /**
     * @brief Generate a graphic character
     *
     * @param character Letter represented by uchar
     * @param message Whole message to compare the size between letters. 'character' letter should present
     * @param height Proportion of the height screen that the letter should cover without aliasing
     * @param ftContent Object containing the FreeType lib objects
     * @return GraphicCharacter Texture and local transform of the character
     */
    GraphicCharacter genGraphicCharacter(
        unsigned char character,
        const std::string &message,
        float height,
        GLsizei screenHeight,
        const FTContent &ftContent
    );

    FT_Pos getSmallestHight(const std::string &message, const FontTexturesGenerator::FTContent &ftContent);

    GraphicAlphabet genGraphicAlphabet(const Menu &menu, GLsizei screenHeight, const FTContent &ftContent);

    /**
     * Create and add in _graphicAlphabet a new graphic character.
     * @param character Letter character
     * @param pixelHeight Height of the graphic character in pixels
     * @return GraphicCharacter texture added into _graphicAlphabet
     */
    GraphicCharacter createOrGetGraphicCharacter(
        unsigned char character,
        FT_UInt pixelHeight
    );

    void freeGraphicAlphabetGPUMemory(const GraphicAlphabet &graphicAlphabet);

    FTContent initFreeTypeAndFont(
        const unsigned char *fontData,
        size_t fontDataSize
    );

    void clearFreeTypeRessources(FTContent &ftContent);
}


#endif
