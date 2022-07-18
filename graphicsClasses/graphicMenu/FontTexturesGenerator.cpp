/*
 * File: FontTexturesGenerator.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 avril 2021, 10:00
 */

#include "FontTexturesGenerator.h"
#include "FrameBuffer.h"
#include "gameMenu/CenteredNode.h"

FontTexturesGenerator::FTContent FontTexturesGenerator::initFreeTypeAndFont(
    const unsigned char *fontData,
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
    if (ftOpenFaceResult != 0) {
        std::cerr << "Error: Impossible to init font face ..." << std::endl;
    }
    return ftContent;
}

void FontTexturesGenerator::clearFreeTypeRessources(FontTexturesGenerator::FTContent &ftContent) {
    FT_Done_Face(ftContent.fontFace);
    FT_Done_FreeType(ftContent.ftLib);
}


FontTexturesGenerator::GraphicCharacter FontTexturesGenerator::genGraphicCharacter(
    unsigned char character,
    const std::string &message,
    float height,
    GLsizei screenHeight,
    const FontTexturesGenerator::FTContent &ftContent) {

    FontTexturesGenerator::GraphicCharacter graphicCharacter;
    const auto setPixelSizes =
        [&ftContent](FT_UInt heightPixels) -> void {
            constexpr FT_UInt scalarQuality = 2;
            FT_Set_Pixel_Sizes(ftContent.fontFace, 0, scalarQuality * heightPixels);
        };
    const auto loadCharacter =
        [&ftContent](unsigned char character) -> void {
            if (const auto callback = FT_Load_Char(ftContent.fontFace, character, FT_LOAD_RENDER)) {
                std::cerr << "Error " << callback << ": Impossible to load glyph " << character << std::endl;
            }
        };
    const auto getMinHeight =
        [&loadCharacter](const std::string &message,
                         const FT_Face &fontFace) -> FT_Pos {
            if (message.size() > 0) {
                loadCharacter(message.at(0));
            } else {
                return 0;
            }
            FT_Pos minHeight = fontFace->glyph->metrics.height;
            fontFace->glyph
            for (const char character: message) {
                loadCharacter(character);
                const FT_Pos characterHeight = fontFace->glyph->metrics.height;
                if (minHeight > characterHeight && characterHeight > 0) {
                    minHeight = characterHeight;
                }
            }
            return minHeight;
        };

    const auto &glyph = ftContent.fontFace->glyph;
    const auto &metrics = glyph->metrics;
    const auto &bitmap = glyph->bitmap;

    const FT_UInt heightPixels = static_cast <FT_UInt>(screenHeight * height);

    setPixelSizes(heightPixels);
    const FT_Pos minHeight = getMinHeight(message, ftContent.fontFace);

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

    const FontTexturesGenerator::CharacterLocalTransform charTransform{
        {scaleX, scaleY},
        {0,      translationY}};

    graphicCharacter.transform = charTransform;

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    loadCharacter(character);
    const GLuint textureID = FrameBuffer::createTexture();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, bitmap.width, bitmap.rows);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        bitmap.width,
        bitmap.rows,
        GL_RED,
        GL_UNSIGNED_BYTE,
        bitmap.buffer
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    graphicCharacter.textureID = textureID;

    return graphicCharacter;
}

FontTexturesGenerator::GraphicAlphabet FontTexturesGenerator::genGraphicAlphabet(
    const Menu &menu,
    GLsizei screenHeight,
    const FontTexturesGenerator::FTContent &ftContent
) {
    FontTexturesGenerator::GraphicAlphabet graphicAlphabet;

    const auto getBiggestHeight =
        [&menu]() {
            float biggestHeight = 0.f;
            for (const auto &page: menu.pages()) {
                for (const auto &label: page->labels()) {
                    const float labelHeight = label->height();
                    if (biggestHeight < labelHeight) {
                        biggestHeight = labelHeight;
                    }
                }
            }
            return biggestHeight;
        };
    const float height = getBiggestHeight();

    for (const auto &page: menu.pages()) {
        for (const auto &label: page->labels()) {
            const std::string message = label->message();
            for (unsigned char character: message) {
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

void FontTexturesGenerator::freeGraphicAlphabetGPUMemory(const GraphicAlphabet &graphicAlphabet) {
    for (const auto &character: graphicAlphabet) {
        const auto &graphicCharacter = character.second;
        glDeleteTextures(1, &graphicCharacter.textureID);
    }
}

FontTexturesGenerator::GraphicCharacter FontTexturesGenerator::createOrGetGraphicCharacter(
    unsigned char character,
    FT_UInt pixelHeight
) {

    const auto hash = MessageLabel::createLetterHash(static_cast<size_t>(pixelHeight), character);
    const auto &iterator = _graphicAlphabet.find(hash);
    if (iterator != _graphicAlphabet.end()) {
        return iterator->second;
    }

    if (const auto callback = FT_Load_Char(_ftContent.fontFace, character, FT_LOAD_RENDER)) {
        std::cerr << "Error " << callback << ": Impossible to load glyph " << character << std::endl;
    }

    const auto &glyph = _ftContent.fontFace->glyph;
    const auto &bitmap = glyph->bitmap;
    const auto width = static_cast<GLsizei>(bitmap.width);
    const auto rows = static_cast<GLsizei>(bitmap.rows);

    const GLuint textureID = FrameBuffer::createTexture();
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, width, rows);
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        width,
        rows,
        GL_RED,
        GL_UNSIGNED_BYTE,
        bitmap.buffer
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    const auto advance = glyph->advance.x >> 6;


    const FontTexturesGenerator::GraphicCharacter graphicCharacter = {
        textureID,
        {
            bitmap.width,
            bitmap.rows,
            static_cast<unsigned int>(glyph->bitmap_left),
            static_cast<unsigned int>(glyph->bitmap_top),
            static_cast<unsigned int>(advance)
        }
    };
    _graphicAlphabet[hash] = graphicCharacter;
    return graphicCharacter;
}

vecMessageLabel_sptr FontTexturesGenerator::genMessageLabels(
    const FontTexturesGenerator::NodeMessageAssociations &nodeToMessage
) {

    vecMessageLabel_sptr messageLabels{};
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (const auto &item: nodeToMessage) {
        const auto &node = item.first;
        const float screenSpaceHeight = node->getScreenSpaceHeight();
        const auto nodePixelHeight = static_cast<FT_UInt>(
            std::ceil(static_cast<float>(_screenHeight) / screenSpaceHeight)
        );

        FT_Set_Pixel_Sizes(_ftContent.fontFace, 0, nodePixelHeight);
        const auto &message = item.second;
        unsigned int nodePixelWidth = 0;
        std::vector<MessageLabel::CharacterLocalTransform> transforms;
        for (unsigned char c: message) {
            const auto graphicCharacter = createOrGetGraphicCharacter(c, nodePixelHeight);
            nodePixelWidth += graphicCharacter.transform.advance;
            transforms.push_back(graphicCharacter.transform);
        }
        const Node_sptr centeredNode = std::make_shared<CenteredNode>(
            node,
            static_cast<float>(nodePixelWidth) / static_cast<float>(nodePixelHeight)
        );
        messageLabels.push_back(std::make_shared<MessageLabel>(
            message,
            std::move(transforms),
            centeredNode,
            nodePixelHeight,
            true // TODO change it
        ));

    }
    return messageLabels;
}

FontTexturesGenerator::FontTexturesGenerator(
    size_t screenWidth,
    size_t screenHeight,
    const FontTexturesGenerator::FTContent &ftContent,
    const FontTexturesGenerator::NodeMessageAssociations &nodeToMessage
) : _ftContent(ftContent),
    _screenWidth(screenWidth),
    _screenHeight(screenHeight),
    _graphicAlphabet{},
    _messageLabels(genMessageLabels(nodeToMessage)) {

}
