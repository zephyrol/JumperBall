/*
 * File: TextRendering.cpp
 * Author: Morgenthaler S
 *
 * Created on ? mai 2020, ??:??
 */

#include "TextRendering.h"
#include "ShaderProgram.h"
#include <iterator>

TextRendering::TextRendering(const Label& label):
    LabelRendering(label),
    _charactersTransforms(label.message().size()),
    _charactersTextureIDs() {
    updateQuad();
    updateAlphabets(label);
    updateAlphabetCharactersIds();
    fillTextureIDs();
    createShader();
}


void TextRendering::updateQuad() {
    if (!displayQuad) {
        displayQuad = std::make_shared <const Quad>();
    }
}

void TextRendering::createShader() {
    if (!spFont) {
        spFont = std::make_shared <const ShaderProgram>(
            Shader(GL_VERTEX_SHADER, vsshaderFont),
            Shader(GL_FRAGMENT_SHADER, fsshaderFont));
    }
}

bool TextRendering::initFreeTypeAndFont() {

    if (FT_Init_FreeType(&ftLib)) {
        std::cerr << "Error: Impossible to init FreeType Lib..." << std::endl;
        return false;
    }

    std::string fontFileToOpenV1 = "fonts/Cousine-Regular.ttf";
    std::string fontFileToOpenV2 = "bin/fonts/Cousine-Regular.ttf";
    const std::vector <std::string> fileNames {
        std::move(fontFileToOpenV1), std::move(fontFileToOpenV2) };

    bool foundFile = false;
    for (size_t i = 0; i < fileNames.size() && !foundFile; ++i) {
        if (
            FT_New_Face(ftLib, fileNames.at(i).c_str(),
                        0, &fontFace) == 0
            ) {
            foundFile = true;
        }
    }
    if (!foundFile) {
        std::cerr << "Error: Impossible to load the font" <<
            "Cousine-Regular.ttf ... " << std::endl;
        JBTypesMethods::displayInstallError();
        FT_Done_FreeType(ftLib);
        return false;
    } else {
        return true;
    }

}

void TextRendering::clearFreeTypeRessources() {
    FT_Done_Face(fontFace);
    FT_Done_FreeType(ftLib);
}

const std::vector <GLuint>& TextRendering::getAlphabetCharactersIds() {
    return alphabetCharactersIds;
}

void TextRendering::fillTextureIDs() {
    for (const char c : _label.message()) {
        const TextRendering::AlphabetTextureKey alphabetTextureKey
        { c, getHeightInPixels() };
        _charactersTextureIDs.push_back(
            alphabetTextures.at(alphabetTextureKey));
    }
}

FT_UInt TextRendering::getHeightInPixels() const {
    return getHeightInPixels(_label);
}

FT_UInt TextRendering::getHeightInPixels (const Label& label) {
    const FT_UInt heightPixels = static_cast <FT_UInt>
                                 (Utility::windowResolutionY * label.height());
    return heightPixels;
}

void TextRendering::render() const {
    const glm::vec3& textColor = getTextColor();
    spFont->bindUniform("fontColor", textColor);
    for (size_t i = 0; i < _label.message().size(); ++i) {
        spFont->bindUniformTexture("characterTexture", 0,
                                   _charactersTextureIDs.at(i));
        spFont->bindUniform("M", _charactersTransforms.at(i));
        displayQuad->draw();
    }
}

void TextRendering::render (size_t index) const {
    spFont->bindUniform("M", _charactersTransforms.at(index));
    displayQuad->draw();
}

std::vector <size_t> TextRendering::getIndicesWithID (GLuint characterId) const {
    std::vector <size_t> indicesWithID;

    for (size_t i = 0; i < _charactersTextureIDs.size(); ++i) {
        if (characterId == _charactersTextureIDs.at(i)) {
            indicesWithID.push_back(i);
        }
    }
    return indicesWithID;
}

const glm::vec3& TextRendering::getTextColor() const {
    return _label.isActivated()
           ? enabledLetterColor
           : disabledLetterColor;
}

void TextRendering::update (float offset) {
    const auto getPosition = [this] (float offset) {
                                 JBTypes::vec2f position = _label.position();
                                 if (!_label.isFixed()) {
                                     position.y += offset;
                                 }
                                 return position;
                             };

    const JBTypes::vec2f position = getPosition(offset);
    const float pitch = _label.width() / _label.message().size();

    constexpr float biasScalar = 2.f; // To multiply the translation by 2
    const float initialOffsetX = -_label.width() / 2.f + pitch / 2.f;
    for (size_t i = 0; i < _label.message().size(); ++i) {
        const float offsetX = initialOffsetX + i * pitch;
        const char c = _label.message().at(i);
        const CharacterTransform& localTransform = alphabetTransforms.at(c);

        const float scaleX = pitch * localTransform.localScale.x;
        const float scaleY = _label.height() * localTransform.localScale.y;
        const glm::vec3 scale = glm::vec3 { scaleX, scaleY, 0.f };

        const glm::mat4 scaleMatrix = glm::scale(scale);
        const float translateX = position.x + offsetX;
        const float translateY = position.y +
                                 localTransform.localTranslate.y * _label.height();
        const glm::vec3 translateVec { translateX, translateY, 0.f };

        const glm::mat4 translate = glm::translate(biasScalar * translateVec);
        _charactersTransforms.at(i) = biasMatrix * translate * scaleMatrix;

    }
}

const std::shared_ptr <const ShaderProgram>& TextRendering::getShaderProgram() {
    return spFont;
}

GLuint TextRendering::getQuadVAO() const {
    // displayQuad can not be null because the pointer is allocated in the
    // constructor
    // displayQuad->vertexArrayObject() can not be null because the pointer is
    // allocated in the GeometryShape constructor
    return *displayQuad->vertexArrayObject();
}

const Quad& TextRendering::getDisplayQuad() const {
    // displayQuad can not be null because the pointer is allocated in the
    // constructor
    return *displayQuad;
}

void TextRendering::updateAlphabets (const Label& label) {
    const auto setPixelSizes = [] (FT_UInt heightPixels)->void {
                                   constexpr FT_UInt scalarQuality = 2;
                                   FT_Set_Pixel_Sizes(fontFace, 0, scalarQuality * heightPixels);
                               };
    const auto loadCharacter = [] (unsigned char character)->void {
                                   if (
                                       const auto callback =
                                           FT_Load_Char(fontFace, character, FT_LOAD_RENDER)
                                       ) {
                                       std::cerr << "Error " << callback
                                                 << ": Impossible to load glyph " << character
                                                 << std::endl;
                                   }
                               };
    const auto getMinHeight = [&loadCharacter] (const std::string& message,
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

    const auto& glyph = fontFace->glyph;
    const auto& metrics = glyph->metrics;
    const auto& bitmap = glyph->bitmap;

    const FT_UInt heightPixels = getHeightInPixels(label);
    setPixelSizes(heightPixels);
    const FT_Pos minHeight = getMinHeight(label.message(), fontFace);

    for (const char character : label.message()) {
        // Transform
        if (alphabetTransforms.find(character) == alphabetTransforms.end()) {
            loadCharacter(character);
            const float metricsWidth = static_cast <float>(metrics.width);
            const float bearingX = static_cast <float>(metrics.horiBearingX);
            const float scaleX = 1.f / (1.f + (bearingX / metricsWidth));

            const float metricsHeight = static_cast <float>(metrics.height);
            const float bearingY = static_cast <float>(metrics.horiBearingY);
            const float fMinHeight = static_cast <float>(minHeight);

            const float scaleY = metricsHeight / fMinHeight;
            const float translationY =
                (bearingY - metricsHeight / 2.f) / fMinHeight - 1.f / 2.f;

            const TextRendering::CharacterTransform charTransform {
                { scaleX, scaleY },
                { 0, translationY }};

            alphabetTransforms[character] = charTransform;
        }

        // Texture
        const TextRendering::AlphabetTextureKey keyAlphaTex
        { character, heightPixels };
        if (alphabetTextures.find(keyAlphaTex) == alphabetTextures.end()) {

            const std::function <GLuint()> genTexture = [] ()->GLuint {
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
            glTexImage2D(GL_TEXTURE_2D, levelOfDetail, GL_RED,
                         bitmap.width,
                         bitmap.rows, 0, GL_RED,
                         GL_UNSIGNED_BYTE,
                         bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            alphabetTextures[keyAlphaTex] = textureID;
        }
    }
}

void TextRendering::updateAlphabetCharactersIds() {
    alphabetCharactersIds.clear();
    for (
        TextRendering::AlphabetTextures::const_iterator itAlphabet =
            TextRendering::alphabetTextures.begin();
        itAlphabet != alphabetTextures.end();
        ++itAlphabet
        ) {
        alphabetCharactersIds.push_back(itAlphabet->second);
    }
}

FT_Library TextRendering::ftLib;
FT_Face TextRendering::fontFace;

TextRendering::AlphabetTextures TextRendering::alphabetTextures {};
TextRendering::AlphabetTransforms TextRendering::alphabetTransforms {};
std::vector <GLuint> TextRendering::alphabetCharactersIds;

std::shared_ptr <const Quad> TextRendering::displayQuad = nullptr;
const glm::vec3 TextRendering::enabledLetterColor =
    glm::vec3(0.f, 1.f, 1.f);
const glm::vec3 TextRendering::disabledLetterColor =
    glm::vec3(0.5f, 0.5f, 0.5f);

const std::string TextRendering::vsshaderFont = "shaders/fontVs.vs";
const std::string TextRendering::fsshaderFont = "shaders/fontFs.fs";

std::shared_ptr <const ShaderProgram> TextRendering::spFont = nullptr;
