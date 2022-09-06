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
    size_t screenWidth, size_t screenHeight,
    const CstPage_sptr &page,
    const FontTexturesGenerator::FTContent &ftContent,
    const FontTexturesGenerator::LettersTexture &lettersTexture,
    vecMessageLabel_sptr &&messageLabels
) : _ftContent(ftContent),
    _screenWidth(screenWidth),
    _screenHeight(screenHeight),
    _lettersTexture(lettersTexture),
    _messageLabels(std::move(messageLabels)) {
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
    glDeleteTextures(1, &_lettersTexture.textureID);
}


FontTexturesGenerator::GraphicCharacter FontTexturesGenerator::createOrGetGraphicCharacter(
    size_t screenWidth,
    const FontTexturesGenerator::FTContent &ftContent,
    FT_UInt pixelHeight,
    LettersTexture &lettersTexture,
    unsigned char character
) {

    const auto hash = MessageLabel::createLetterHash(static_cast<size_t>(pixelHeight), character);
    const auto &iterator = lettersTexture.graphicAlphabet.find(hash);
    if (iterator != lettersTexture.graphicAlphabet.end()) {
        return iterator->second;
    }

    if (const auto callback = FT_Load_Char(ftContent.fontFace, character, FT_LOAD_RENDER)) {
        std::cerr << "Error " << callback << ": Impossible to load glyph " << character << std::endl;
    }

    const auto &glyph = ftContent.fontFace->glyph;
    const auto &bitmap = glyph->bitmap;
    const auto width = static_cast<GLsizei>(bitmap.width);
    const auto rows = static_cast<GLsizei>(bitmap.rows);

    const auto advance = glyph->advance.x >> 6;

    const auto pixelsUv = insertCharacterToTexture(
        screenWidth,
        lettersTexture,
        bitmap.buffer,
        width,
        rows
    );

    const FontTexturesGenerator::GraphicCharacter graphicCharacter = {
        {pixelsUv.x, pixelsUv.y},
        {pixelsUv.z, pixelsUv.w},
        {bitmap.width, bitmap.rows},
        {glyph->bitmap_left, glyph->bitmap_top},
        static_cast<unsigned int>(advance)
    };
    lettersTexture.graphicAlphabet[hash] = graphicCharacter;
    return graphicCharacter;
}

std::vector<MessageLabel::CharacterLocalTransform> FontTexturesGenerator::getCharacterLocalTransforms(
    const std::vector<GraphicCharacter> &graphicCharacters,
    unsigned int nodePixelWidth,
    unsigned int nodePixelHeight
) {

    std::vector<MessageLabel::CharacterLocalTransform> localTransforms{};
    const auto fNodePixelWidth = static_cast<float>(nodePixelWidth);
    const auto fNodePixelHeight = static_cast<float>(nodePixelHeight);

    localTransforms.reserve(graphicCharacters.size());
    for (const auto &graphicCharacter: graphicCharacters) {
        localTransforms.push_back(
            {
                static_cast<float>(graphicCharacter.size.x) / fNodePixelWidth,
                static_cast<float>(graphicCharacter.size.y) / fNodePixelHeight,
                static_cast<float>(graphicCharacter.bearing.x) / fNodePixelWidth,
                static_cast<float>(graphicCharacter.bearing.y) / fNodePixelHeight,
                static_cast<float>(graphicCharacter.advance) / fNodePixelWidth,
                static_cast<float>(graphicCharacter.pixelsUvPixelsMin.x) / fNodePixelWidth,
                static_cast<float>(graphicCharacter.pixelsUvPixelsMin.y) / fNodePixelHeight,
                static_cast<float>(graphicCharacter.pixelsUvPixelsMax.x) / fNodePixelWidth,
                static_cast<float>(graphicCharacter.pixelsUvPixelsMax.y) / fNodePixelHeight
            }
        );
    }
    return localTransforms;
}

const vecMessageLabel_sptr &FontTexturesGenerator::getMessageLabels() {
    return _messageLabels;
}

FontTexturesGenerator FontTexturesGenerator::createInstance(
    size_t screenWidth,
    size_t screenHeight,
    const CstPage_sptr &page,
    const FontTexturesGenerator::FTContent &ftContent
) {

    FontTexturesGenerator::LettersTexture lettersTexture;

    // 1. Generate message labels
    vecMessageLabel_sptr messageLabels{};
    const auto &nodeToMessage = page->nodeToMessage();
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (const auto &item: nodeToMessage) {
        const auto &node = item.first;
        const auto screenSpaceHeight = node->height();
        const auto nodePixelHeight = static_cast<FT_UInt>(
            std::ceil(static_cast<float>(screenHeight) * screenSpaceHeight)
        );

        FT_Set_Pixel_Sizes(ftContent.fontFace, 0, nodePixelHeight);
        const auto &message = item.second;
        unsigned int nodePixelWidth = 0;
        std::vector<FontTexturesGenerator::GraphicCharacter> transforms;
        for (unsigned char c: message) {
            const auto graphicCharacter = createOrGetGraphicCharacter(
                screenWidth,
                ftContent,
                nodePixelHeight,
                lettersTexture,
                c
            );
            nodePixelWidth += graphicCharacter.advance;
            transforms.push_back(graphicCharacter);
        }
        const Node_sptr centeredNode = std::make_shared<CenteredNode>(
            node,
            static_cast<float>(nodePixelWidth) / static_cast<float>(nodePixelHeight)
        );

        messageLabels.push_back(std::make_shared<MessageLabel>(
            message,
            getCharacterLocalTransforms(transforms, nodePixelWidth, nodePixelHeight),
            centeredNode,
            nodePixelHeight,
            true // TODO change it
        ));

    }

    lettersTexture.textureID = FrameBuffer::createTexture();
    glBindTexture(GL_TEXTURE_2D, lettersTexture.textureID);
    glTexStorage2D(
        GL_TEXTURE_2D,
        1,
        GL_R8,
        static_cast<GLsizei>(lettersTexture.width),
        static_cast<GLsizei>(lettersTexture.height)
    );
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        static_cast<GLsizei>(lettersTexture.width),
        static_cast<GLsizei>(lettersTexture.height),
        GL_RED,
        GL_UNSIGNED_BYTE,
        lettersTexture.lettersData.data()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return FontTexturesGenerator(
        screenWidth,
        screenHeight,
        page,
        ftContent,
        lettersTexture,
        std::move(messageLabels)
    );

}


glm::ivec4 FontTexturesGenerator::insertCharacterToTexture(
    size_t screenWidth,
    FontTexturesGenerator::LettersTexture &lettersTexture,
    const unsigned char *letterBitmap,
    unsigned int bitmapWidth,
    unsigned int bitmapHeight
) {
    const auto drawingCursor = [&]() -> glm::ivec2 {
        if (lettersTexture.cursor.x + bitmapWidth > screenWidth) {
            return {0.f, lettersTexture.height};
        }
        return lettersTexture.cursor;
    }();

    const glm::ivec2 outputSize = {
        std::max(lettersTexture.width, drawingCursor.x + bitmapWidth),
        std::max(lettersTexture.height, drawingCursor.y + bitmapHeight)
    };

    const auto getIndex = [](unsigned int line, unsigned int column, unsigned int width) {
        return line * width + column;
    };

    // Resize letter textures
    if (outputSize != glm::ivec2{lettersTexture.width, lettersTexture.height}) {
        lettersTexture.lettersData = [&]() {
            std::vector<unsigned char> resized(outputSize.x * outputSize.y, 0);
            for (unsigned int i = 0; i < lettersTexture.height; ++i) {
                for (unsigned int j = 0; j < lettersTexture.width; ++j) {
                    const auto baseIndex = getIndex(i, j, lettersTexture.width);
                    const auto value = lettersTexture.lettersData[baseIndex];
                    const auto resizedDataIndex = getIndex(i, j, outputSize.x);
                    resized[resizedDataIndex] = value;
                }
            }
            return resized;
        }();
        lettersTexture.width = outputSize.x;
        lettersTexture.height = outputSize.y;
    }

    // Write bitmap into texture
    for (unsigned int i = 0; i < bitmapHeight; ++i) {
        for (unsigned int j = 0; j < bitmapWidth; ++j) {
            const auto baseIndex = getIndex(i, j, bitmapWidth);
            const auto value = letterBitmap[baseIndex];
            const auto targetIndex = getIndex(
                drawingCursor.y + i,
                drawingCursor.x + j,
                outputSize.x
            );
            lettersTexture.lettersData[targetIndex] = value;
        }
    }

    // Update cursor
    lettersTexture.cursor = {
        drawingCursor.x + bitmapWidth,
        drawingCursor.y
    };
    return {drawingCursor.x, drawingCursor.y, drawingCursor.x + bitmapWidth, drawingCursor.y + bitmapHeight};
}

GLuint FontTexturesGenerator::getLettersTexture() const {
    return _lettersTexture.textureID;
}
