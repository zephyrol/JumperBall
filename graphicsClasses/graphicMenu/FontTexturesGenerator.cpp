/*
 * File: FontTexturesGenerator.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 avril 2021, 10:00
 */

#include "FontTexturesGenerator.h"

#include <utility>
#include "frameBuffer/FrameBuffer.h"
#include "gameMenu/nodes/CenteredNode.h"

FontTexturesGenerator::FontTexturesGenerator(
    FontTexturesGenerator::LettersTexture lettersTexture,
    vecTextLabel_sptr &&textLabels
) : _lettersTexture(std::move(lettersTexture)),
    _messageLabels(std::move(textLabels)) {
}


FontTexturesGenerator::FTContent FontTexturesGenerator::initFreeTypeAndFont(
    const unsigned char *fontData,
    size_t fontDataSize
) {

    FontTexturesGenerator::FTContent ftContent{};

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

void FontTexturesGenerator::clearFreeTypeResources(FontTexturesGenerator::FTContent &ftContent) {
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

    const auto hash = TextLabel::createLetterHash(static_cast<size_t>(pixelHeight), character);
    const auto &iterator = lettersTexture.graphicAlphabet.find(hash);
    if (iterator != lettersTexture.graphicAlphabet.end()) {
        return iterator->second;
    }

    const auto getCharCode = [character]() -> FT_ULong  {
        if(character == ';') {
            return 233; // e with a ' (french)
        }
        return character;
    };

    if (const auto callback = FT_Load_Char(ftContent.fontFace, getCharCode(), FT_LOAD_RENDER)) {
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

std::vector<TextLabel::CharacterLocalTransform> FontTexturesGenerator::getCharacterLocalTransforms(
    const std::vector<GraphicCharacter> &graphicCharacters,
    unsigned int nodePixelWidth,
    unsigned int nodePixelHeight
) {

    std::vector<TextLabel::CharacterLocalTransform> localTransforms{};
    const auto fNodePixelWidth = static_cast<float>(nodePixelWidth);
    const auto fNodePixelHeight = static_cast<float>(nodePixelHeight);

    localTransforms.reserve(graphicCharacters.size());
    for (const auto &graphicCharacter: graphicCharacters) {
        localTransforms.push_back(
            {
                static_cast<float>(graphicCharacter.size.x) / fNodePixelWidth,
                static_cast<float>(graphicCharacter.size.y) / fNodePixelHeight,
                static_cast<float>(graphicCharacter.bearing.x) / fNodePixelWidth,
                static_cast<float>(graphicCharacter.size.y - graphicCharacter.bearing.y) / fNodePixelHeight,
                static_cast<float>(graphicCharacter.advance) / fNodePixelWidth,
            }
        );
    }
    return localTransforms;
}

const vecTextLabel_sptr &FontTexturesGenerator::getTextLabels() {
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
    vecTextLabel_sptr textLabels{};
    auto lettersUvs = std::make_shared<TextLabel::LettersUvs>();
    auto textNodes = page->genTextNodes();
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (auto &textNode: textNodes) {
        const auto &node = textNode->node();
        const auto screenSpaceHeight = node->height();
        const auto nodePixelHeight = static_cast<FT_UInt>(
            std::ceil(static_cast<float>(screenHeight) * screenSpaceHeight)
        );

        FT_Set_Pixel_Sizes(ftContent.fontFace, 0, nodePixelHeight);
        const auto &message = textNode->text();
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

        auto textLabel =  std::make_shared<TextLabel>(
            std::move(textNode),
            centeredNode,
            lettersUvs,
            getCharacterLocalTransforms(transforms, nodePixelWidth, nodePixelHeight),
            nodePixelHeight,
            JBTypes::Color::Blue
        );
        textLabels.push_back(std::move(textLabel));
    }

    for (const auto &letterTransform: lettersTexture.graphicAlphabet) {
        const auto &letter = letterTransform.first;
        const auto &transform = letterTransform.second;
        (*lettersUvs)[letter] = {
            static_cast<float>(transform.pixelsUvPixelsMin.x) / static_cast<float>(lettersTexture.width),
            static_cast<float>(transform.pixelsUvPixelsMin.y) / static_cast<float>(lettersTexture.height),
            static_cast<float>(transform.pixelsUvPixelsMax.x) / static_cast<float>(lettersTexture.width),
            static_cast<float>(transform.pixelsUvPixelsMax.y) / static_cast<float>(lettersTexture.height)
        };
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

    return {
        lettersTexture,
        std::move(textLabels)
    };
}


glm::ivec4 FontTexturesGenerator::insertCharacterToTexture(
    size_t screenWidth,
    FontTexturesGenerator::LettersTexture &lettersTexture,
    const unsigned char *letterBitmap,
    unsigned int bitmapWidth,
    unsigned int bitmapHeight
) {

    // Add a blank line and column to avoid neighbor pixel picking through linear
    const auto extendedWidth = bitmapWidth + 1;
    const auto extendedHeight = bitmapHeight + 1;

    const auto drawingCursor = [&]() -> glm::ivec2 {
        if (lettersTexture.cursor.x + extendedWidth > screenWidth) {
            return {0.f, lettersTexture.height};
        }
        return lettersTexture.cursor;
    }();

    const glm::ivec2 outputSize = {
        std::max(lettersTexture.width, drawingCursor.x + extendedWidth),
        std::max(lettersTexture.height, drawingCursor.y + extendedHeight)
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
            // First bytes of opengl textures represents the bottom of the texture
            const auto baseIndex = getIndex(bitmapHeight - i - 1, j, bitmapWidth);
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
        drawingCursor.x + extendedWidth,
        drawingCursor.y
    };
    return {
        drawingCursor.x,
        drawingCursor.y,
        drawingCursor.x + bitmapWidth,
        drawingCursor.y + bitmapHeight
    };
}

GLuint FontTexturesGenerator::getLettersTexture() const {
    return _lettersTexture.textureID;
}
