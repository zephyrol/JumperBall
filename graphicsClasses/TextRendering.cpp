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

TextRendering::TextRendering(const std::vector<unsigned char>& characters,
                             unsigned int height):
    _alphabet(initAlphabet(characters,height)),
    _fontHeight(height),
    _displayQuad()
{

}

bool TextRendering::initFreeTypeAndFont() {

    if (FT_Init_FreeType(&ftLib)) {
        std::cerr << "Error: Impossible to init FreeType Lib..." << std::endl;
        return false;
    }

    if(FT_New_Face(ftLib,"fonts/Cousine-Regular.ttf",
                   0,&fontFace)){
        std::cerr << "Error: Impossible to load the font" <<
                     "Cousine-Regular.ttf ... " << std::endl;
        Utility::displayInstallError();
        FT_Done_FreeType(ftLib);
        return false;
    }
    return true;

}

void TextRendering::clearFreeTypeRessources() {
    FT_Done_Face(fontFace);
    FT_Done_FreeType(ftLib);
}

std::map<unsigned char, TextRendering::Character> TextRendering::initAlphabet(
        const std::vector<unsigned char>& characters, unsigned int height)
{
    std::map<unsigned char, TextRendering::Character> alphabet;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    FT_Set_Pixel_Sizes(fontFace,0,height);
    for (const unsigned char& character : characters) {

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

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            const TextRendering::Character graphicChar {
                textureID,
                {fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows},
                {fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top}
            };
            alphabet[character] = graphicChar;
        }
        else {
            std::cerr << "Error... Impossible to load glyph " << character <<
                         std::endl;
        }
    }
    return alphabet;
}

void TextRendering::render( const ShaderProgram& sp, const MessageLabel& label, 
                            const std::pair<float,float>& position,
                            const glm::vec3& color) const {
    sp.use();

    const float pitch = label.width()/label.message().size();
    const glm::mat4 scale = glm::scale( glm::vec3{pitch, label.height(),0.f});
    float offsetX = -label.width()/2.f + pitch/2.f;
    const glm::mat4 biasMatrix  = glm::mat4{ 1.f, 0.f,  0.f, 0.f,
                                             0.f,  1.f, 0.f, 0.f,
                                             0.f,  0.f,  1.f, 0.f,
                                             -1.f, -1.f, 0.f, 1.f} ;

    _displayQuad.bind();
    for (const char& c : label.message()) {
        constexpr float biasScalar = 2.f; //To multiply the translation by 2
        const glm::mat4 translate = glm::translate(
                                biasScalar * glm::vec3{ position.first+offsetX,
                                position.second,0.f});
        const glm::mat4 transformCharacter = biasMatrix * translate * scale;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _alphabet.at(c).texture);
        sp.bindUniformTexture("characterTexture", 0);
        sp.bindUniform("fontColor",color);
        sp.bindUniform("M",transformCharacter);
        _displayQuad.draw();
        offsetX += pitch;
    }
}




FT_Library TextRendering::ftLib;
FT_Face TextRendering::fontFace;
