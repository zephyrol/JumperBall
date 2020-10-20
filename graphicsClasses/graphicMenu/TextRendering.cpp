/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TextRendering.cpp
 * Author: Morgenthaler S
 * 
 * Created on ? mai 2020, ??:??
 */

#include "TextRendering.h"
#include "ShaderProgram.h"

TextRendering::TextRendering(const Label &label,
                             float maxHeight,
                             const ShaderProgram &spFont):
    LabelRendering(label),
    _spFont(spFont),
    _charactersTransforms(label.message().size())
{
    updateQuad();
    updateCharacters(label,maxHeight);
}


void TextRendering::updateQuad() {
    if (!displayQuad) {
        displayQuad = std::make_shared<const Quad>();
    }
}

bool TextRendering::initFreeTypeAndFont() {

    if (FT_Init_FreeType(&ftLib)) {
        std::cerr << "Error: Impossible to init FreeType Lib..." << std::endl;
        return false;
    }

    std::string fontFileToOpenV1 = "fonts/Cousine-Regular.ttf";
    std::string fontFileToOpenV2 = "bin/fonts/Cousine-Regular.ttf";
    const std::vector<std::string> fileNames {
        std::move(fontFileToOpenV1), std::move(fontFileToOpenV2)};

    bool foundFile = false;
    for (size_t i = 0; i < fileNames.size() && !foundFile; ++i) {
        if(FT_New_Face(ftLib,fileNames.at(i).c_str(),
                   0,&fontFace) == 0) {
            foundFile = true;
        }
    }
    if (!foundFile) {
        std::cerr << "Error: Impossible to load the font" <<
                     "Cousine-Regular.ttf ... " << std::endl;
        JBTypesMethods::displayInstallError();
        FT_Done_FreeType(ftLib);
        return false;
    } else  {
        return true;
    }

}

void TextRendering::clearFreeTypeRessources() {
    FT_Done_Face(fontFace);
    FT_Done_FreeType(ftLib);
}

void TextRendering::render() const {
    const glm::vec3& textColor = _label.isActivated() 
        ? enabledLetterColor
        : disabledLetterColor;
    _spFont.bindUniform("fontColor",textColor);
    for (size_t i = 0; i < _label.message().size(); ++i) {
        const char c = _label.message().at(i);
        _spFont.bindUniformTexture("characterTexture", 0,
                                   alphabet.at(c).texture);
        _spFont.bindUniform("M",_charactersTransforms.at(i));
        displayQuad->draw();
    }
}

void TextRendering::update(float offset) {

    JBTypes::vec2f position = _label.position();
    if (!_label.isFixed()) {
        position.y += offset;
    }

    const float pitch = _label.width()/_label.message().size();
    float offsetX = -_label.width()/2.f + pitch/2.f;

    constexpr float biasScalar = 2.f; //To multiply the translation by 2
    for ( size_t i = 0; i < _label.message().size(); ++i){
        const char c = _label.message().at(i);
        const glm::vec3 scale = glm::vec3{pitch * alphabet.at(c).localScale.x,
                _label.height() * alphabet.at(c).localScale.y ,0.f};

        const glm::mat4 scaleMatrix = glm::scale(scale);

        const glm::mat4 translate =
           glm::translate( biasScalar *
               glm::vec3{ position.x+ offsetX +
                          alphabet.at(c).localTranslate.x *
                          scale.x,
                           position.y -
                          (1.f-alphabet.at(c).localTranslate.y) * scale.y,
                          0.f}
                         );
        _charactersTransforms.at(i) = biasMatrix * translate * scaleMatrix;

        offsetX += pitch;
    }
}

const ShaderProgram& TextRendering::getShaderProgram() const {
    return _spFont;
}

GLuint TextRendering::getQuadVAO() const 
{
    // displayQuad can not be null because the pointer is allocated in the
    // constructor
    // displayQuad->vertexArrayObject() can not be null because the pointer is
    // allocated in the GeometryShape constructor
    return *displayQuad->vertexArrayObject() ;
}

const Quad& TextRendering::getDisplayQuad() const 
{
    // displayQuad can not be null because the pointer is allocated in the
    // constructor
    return *displayQuad;
}


void TextRendering::updateCharacters(const Label& label, float maxHeight)
{
    const FT_UInt height = Utility::windowResolutionY * maxHeight;
    FT_Set_Pixel_Sizes(fontFace,0,height);
    for (const char& character : label.message()) {
        if ( alphabet.find(character) == alphabet.end()){
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
            if (!FT_Load_Char(fontFace,character,FT_LOAD_RENDER)) {

                GLuint textureID;
                glGenTextures(1,&textureID);
                glBindTexture(GL_TEXTURE_2D,textureID);

                constexpr GLint levelOfDetail = 0;
                glTexImage2D(GL_TEXTURE_2D, levelOfDetail, GL_RED,
                             fontFace->glyph->bitmap.width,
                             fontFace->glyph->bitmap.rows, 0, GL_RED,
                             GL_UNSIGNED_BYTE,
                             fontFace->glyph->bitmap.buffer);

                glTexParameteri(GL_TEXTURE_2D,
                                GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,
                                GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D,
                                GL_TEXTURE_WRAP_S,
                                GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D,
                                GL_TEXTURE_WRAP_T,
                                GL_CLAMP_TO_EDGE);

                float width;
                if (character == ' ') {
                    width = static_cast<float>(height);
                } else {
                    width = static_cast<float>(height) *
                            fontFace->glyph->bitmap.width /
                            fontFace->glyph->bitmap.rows;
                }
                const TextRendering::Character graphicChar {
                    textureID,
                    {fontFace->glyph->bitmap.width/width,
                     fontFace->glyph->bitmap.rows/static_cast<float>(height)},
                    {fontFace->glyph->bitmap_left/width,
                     fontFace->glyph->bitmap_top/static_cast<float>(height)}
                };
                alphabet[character] = graphicChar;
            }
            else {
                std::cerr << "Error... Impossible to load glyph " << character
                          << std::endl;
            }
        }
    }
}

FT_Library TextRendering::ftLib;
FT_Face TextRendering::fontFace;

std::map<unsigned char, TextRendering::Character> TextRendering::alphabet{};

std::shared_ptr<const Quad> TextRendering::displayQuad = nullptr;
const glm::vec3 TextRendering::enabledLetterColor = 
    glm::vec3(0.f,1.f,1.f);
const glm::vec3 TextRendering::disabledLetterColor = 
    glm::vec3(0.5f,0.5f,0.5f);
