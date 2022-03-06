/*
 * File: FontTexturesGenerator.h
 * Author: Morgenthaler S
 *
 * Created on 22 avril 2021, 10:00
 */

#ifndef FONT_TEXTURES_GENERATOR_H
#define FONT_TEXTURES_GENERATOR_H
#include "Utility.h"
#include <include/ft2build.h>
#include FT_FREETYPE_H
#include <gameMenu/Menu.h>

namespace FontTexturesGenerator {

struct CharacterTransform {
    glm::vec2 localScale;
    glm::vec2 localTranslate;
};
struct GraphicCharacter {
    GLuint textureID;
    CharacterTransform characterTransform;
};
struct FTContent {
    FT_Library ftLib;
    FT_Face fontFace;
};

using GraphicAlphabet = std::unordered_map <unsigned char, GraphicCharacter>;

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
    const std::string& message,
    float height,
    GLsizei screenHeight,
    const FTContent& ftContent);

FT_Pos getSmallestHight(const std::string& message, const FontTexturesGenerator::FTContent& ftContent);
GraphicAlphabet genGraphicAlphabet(const Menu& menu, GLsizei screenHeight, const FTContent& ftContent);

void freeGraphicAlphabetGPUMemory(const GraphicAlphabet& graphicAlphabet);

FTContent initFreeTypeAndFont(
    const unsigned char* fontData,
    size_t fontDataSize
);
    void clearFreeTypeRessources(FTContent& ftContent);
}


#endif