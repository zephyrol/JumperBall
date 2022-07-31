/*
 * File: FontTexturesGenerator.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 avril 2021, 10:00
 */

#include "FontTexturesGenerator.h"
#include "FrameBuffer.h"
#include "gameMenu/CenteredNode.h"

FontTexturesGenerator::FontTexturesGenerator(
    size_t screenWidth,
    size_t screenHeight,
    const CstPage_sptr &page,
    const FontTexturesGenerator::FTContent &ftContent
) : _ftContent(ftContent),
    _screenWidth(screenWidth),
    _screenHeight(screenHeight),
    _graphicAlphabet{},
    _messageLabels(genMessageLabels(page)) {
}


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
    openArgs.memory_size = static_cast<FT_Long>(fontDataSize);
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

void FontTexturesGenerator::freeGPUMemory() {
    for (const auto &character: _graphicAlphabet) {
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
        {bitmap.width, bitmap.rows},
        {glyph->bitmap_left, glyph->bitmap_top},
        static_cast<unsigned int>(advance)
    };
    _graphicAlphabet[hash] = graphicCharacter;
    return graphicCharacter;
}


vecMessageLabel_sptr FontTexturesGenerator::genMessageLabels(
    const CstPage_sptr &page
) {

    vecMessageLabel_sptr messageLabels{};
    const auto& nodeToMessage = page->nodeToMessage();
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
        std::vector<FontTexturesGenerator::GraphicCharacter> transforms;
        for (unsigned char c: message) {
            const auto graphicCharacter = createOrGetGraphicCharacter(c, nodePixelHeight);
            nodePixelWidth += graphicCharacter.advance;
            transforms.push_back(graphicCharacter);
        }
        const Node_sptr centeredNode = std::make_shared<CenteredNode>(
            node,
            static_cast<float>(nodePixelWidth) / static_cast<float>(nodePixelHeight)
        );
        centeredNode->updateScreenTransform();

        messageLabels.push_back(std::make_shared<MessageLabel>(
            message,
            getCharacterLocalTransforms(transforms, nodePixelWidth, nodePixelWidth),
            centeredNode,
            nodePixelHeight,
            true // TODO change it
        ));

    }
    return messageLabels;
}

std::vector<MessageLabel::CharacterLocalTransform> FontTexturesGenerator::getCharacterLocalTransforms(
    const std::vector<GraphicCharacter> &graphicCharacters,
    unsigned int nodePixelWidth,
    unsigned int nodePixelHeight
) {

    std::vector<MessageLabel::CharacterLocalTransform> localTransforms{};
    const auto fNodePixelWidth = static_cast<float>(nodePixelWidth);
    const auto fNodePixelHeight = static_cast<float>(nodePixelHeight);

    for (const auto &graphicCharacter: graphicCharacters) {
        localTransforms.push_back(
            {
                static_cast<float>(graphicCharacter.size.x) / fNodePixelWidth,
                static_cast<float>(graphicCharacter.size.y) / fNodePixelHeight,
                static_cast<float>(graphicCharacter.bearing.x) / fNodePixelWidth,
                static_cast<float>(graphicCharacter.bearing.y) / fNodePixelHeight,
                static_cast<float>(graphicCharacter.advance) / fNodePixelWidth
            }
        );
    }
    return localTransforms;
}

const vecMessageLabel_sptr &FontTexturesGenerator::getMessageLabels() {
    return _messageLabels;
}
