/*
 * File: FontTexturesGenerator.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 avril 2021, 10:00
 */

#include "FontTexturesGenerator.h"

FontTexturesGenerator::FTContent FontTexturesGenerator::initFreeTypeAndFont(
    const unsigned char* fontData,
    size_t fontDataSize
) {

    FontTexturesGenerator::FTContent ftContent;

    if (FT_Init_FreeType(&ftContent.ftLib)) {
        std::cerr << "Error: Impossible to init FreeType Lib..." << std::endl;
        return ftContent;
    }

    FT_Open_Args openArgs;
    openArgs.flags = FT_OPEN_MEMORY;
    openArgs.memory_size = fontDataSize;
    openArgs.memory_base = fontData;

    const auto ftOpenFaceResult = FT_Open_Face(
        ftContent.ftLib,
        &openArgs,
        0,
        &ftContent.fontFace
    );
    if(ftOpenFaceResult != 0) {
        std::cerr << "Error: Impossible to init font face ..." << std::endl;
    }
    return ftContent;
}

void FontTexturesGenerator::clearFreeTypeRessources (FontTexturesGenerator::FTContent& ftContent) {
    FT_Done_Face(ftContent.fontFace);
    FT_Done_FreeType(ftContent.ftLib);
}


FontTexturesGenerator::GraphicCharacter FontTexturesGenerator::genGraphicCharacter (
    unsigned char character,
    const std::string& message,
    float height,
    GLsizei screenHeight,
    const FontTexturesGenerator::FTContent& ftContent) {

    FontTexturesGenerator::GraphicCharacter graphicCharacter;

    const auto setPixelSizes =
        [&ftContent] (FT_UInt heightPixels)->void {
            constexpr FT_UInt scalarQuality = 2;
            FT_Set_Pixel_Sizes(ftContent.fontFace, 0, scalarQuality * heightPixels);
        };
    const auto loadCharacter =
        [&ftContent] (unsigned char character)->void {
            if (const auto callback = FT_Load_Char(ftContent.fontFace, character, FT_LOAD_RENDER)) {
                std::cerr << "Error " << callback << ": Impossible to load glyph " << character << std::endl;
            }
        };
    const auto getMinHeight =
        [&loadCharacter] (const std::string& message,
                          const FT_Face& fontFace)->FT_Pos {
            if (message.size() > 0) {
                loadCharacter(message.at(0));
            } else {
                return 0;
            }
            FT_Pos minHeight = fontFace->glyph->metrics.height;
            for (const char character : message) {
                loadCharacter(character);
                const FT_Pos characterHeight = fontFace->glyph->metrics.height;
                if (minHeight > characterHeight && characterHeight > 0) {
                    minHeight = characterHeight;
                }
            }
            return minHeight;
        };

    const auto& glyph = ftContent.fontFace->glyph;
    const auto& metrics = glyph->metrics;
    const auto& bitmap = glyph->bitmap;

    const FT_UInt heightPixels = static_cast <FT_UInt>(screenHeight * height);

    setPixelSizes(heightPixels);
    const FT_Pos minHeight = getMinHeight( /*{ static_cast <char>(character) }*/ message,
                                           ftContent.fontFace);

    // Transform
    loadCharacter(character);
    const float metricsWidth = static_cast <float>(metrics.width);
    const float bearingX = static_cast <float>(metrics.horiBearingX);
    const float scaleX = 1.f / (1.f + (bearingX / metricsWidth));

    const float metricsHeight = static_cast <float>(metrics.height);
    const float bearingY = static_cast <float>(metrics.horiBearingY);
    const float fMinHeight = static_cast <float>(minHeight);

    const float scaleY = metricsHeight / fMinHeight;
    const float translationY = (bearingY - metricsHeight / 2.f) / fMinHeight - 0.5f;

    const FontTexturesGenerator::CharacterTransform charTransform {
        { scaleX, scaleY },
        { 0, translationY }};

    graphicCharacter.characterTransform = charTransform;

    // Texture
    // TODO: gather this function with FrameBuffer function
    const std::function <GLuint()> genTexture =
        [] ()->GLuint {
            GLuint texture;
            glGenTextures(1, &texture);
            return texture;
        };
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    loadCharacter(character);

    const GLuint textureID = genTexture();
    glBindTexture(GL_TEXTURE_2D, textureID);

    constexpr GLint levelOfDetail = 0;
    glTexImage2D(GL_TEXTURE_2D,
                 levelOfDetail,
                 GL_RED,
                 bitmap.width,
                 bitmap.rows, 0, GL_RED,
                 GL_UNSIGNED_BYTE,
                 bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    graphicCharacter.textureID = textureID;

    return graphicCharacter;
}

FontTexturesGenerator::GraphicAlphabet FontTexturesGenerator::genGraphicAlphabet (
    const Menu& menu,
    GLsizei screenHeight,
    const FontTexturesGenerator::FTContent& ftContent
    ) {
    FontTexturesGenerator::GraphicAlphabet graphicAlphabet;

    const auto getBiggestHeight =
        [&menu] () {
            float biggestHeight = 0.f;
            for (const auto& page : menu.pages()) {
                for (const auto& label : page->labels()) {
                    const float labelHeight = label->height();
                    if (biggestHeight < labelHeight) {
                        biggestHeight = labelHeight;
                    }
                }
            }
            return biggestHeight;
        };
    const float height = getBiggestHeight();

    for (const auto& page : menu.pages()) {
        for (const auto& label : page->labels()) {
            const std::string message = label->message();
            for (unsigned char character : message) {
                if (graphicAlphabet.find(character) == graphicAlphabet.end()) {
                    graphicAlphabet[character] = genGraphicCharacter(
                        character,
                        message,
                        height,
                        screenHeight,
                        ftContent
                    );
                }
            }
        }
    }

    return graphicAlphabet;
}

void FontTexturesGenerator::freeGraphicAlphabetGPUMemory(const GraphicAlphabet &graphicAlphabet)
{
    for (const auto& character: graphicAlphabet) {
        const auto& graphicCharacter = character.second; 
        glDeleteTextures(1, &graphicCharacter.textureID);
    }
}
