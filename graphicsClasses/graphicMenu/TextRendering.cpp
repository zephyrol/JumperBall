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

TextRendering::TextRendering(const Label &label ):
    LabelRendering(label),
    //_alphabet(initAlphabet(characters,height)),
    //_fontHeight(height),
    _displayQuad(),
    _spFont(Shader (GL_VERTEX_SHADER,   vsshaderFont),
            Shader (GL_FRAGMENT_SHADER, fsshaderFont)),
    _charactersTransforms(label.message().size())
{
    updateCharacters(label);
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
            
            float width;
            if (character == ' ') {
                width = static_cast<float>(height);
            } else {
                width = static_cast<float>(height) *
                    fontFace->glyph->bitmap.width/ fontFace->glyph->bitmap.rows;
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
            std::cerr << "Error... Impossible to load glyph " << character <<
                         std::endl;
        }
    }
    return alphabet;
}

void TextRendering::render() const {
    _spFont.use();
    _displayQuad.bind();
    for (size_t i = 0; i < _label.message().size(); ++i) {
        const char& c = _label.message().at(i);
        _spFont.bindUniformTexture("characterTexture", 0,
                                   alphabet.at(c).texture);
        _spFont.bindUniform("fontColor",glm::vec3(1.0,1.0,1.0));
        _spFont.bindUniform("M",_charactersTransforms.at(i));
        _displayQuad.draw();
    }
}

void TextRendering::update(float offset) {

    JBTypes::vec2f position = _label.position();
    if (!_label.isFixed()) {
        position.y += offset;
    }

    const float pitch = _label.width()/_label.message().size();
    float offsetX = -_label.width()/2.f + pitch/2.f;

    //_charactersTransforms.clear();
    constexpr float biasScalar = 2.f; //To multiply the translation by 2
    //for (const char& c : _label.message()) {
    for ( size_t i = 0; i < _label.message().size(); ++i){
        const char& c = _label.message().at(i);
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

/*void TextRendering::render(const CstLabel_sptr& label,
                           const glm::vec3& color ) const {
    _spFont.use();
    _displayQuad.bind();
    for (size_t i = 0; i < label->message().size(); ++i) {
        const char& c = label->message().at(i);
        _spFont.bindUniformTexture("characterTexture", 0,
                                   _alphabet.at(c).texture);
        _spFont.bindUniform("fontColor",color);
        _spFont.bindUniform("M",_charactersTransforms.at(label).at(i));
        _displayQuad.draw();
    }
}*/

/*void TextRendering::update(const CstLabel_sptr&  label, float offsetY)
{
    JBTypes::vec2f position = label->position();
    if (!label->isFixed()) {
        position.y += offsetY;
    }

    const float pitch = label->width()/label->message().size();
    float offsetX = -label->width()/2.f + pitch/2.f;

    _charactersTransforms[label].clear();
    constexpr float biasScalar = 2.f; //To multiply the translation by 2
    for (const char& c : label->message()) {
        const glm::vec3 scale = glm::vec3{pitch * _alphabet.at(c).localScale.x,
                label->height() * _alphabet.at(c).localScale.y ,0.f};

        const glm::mat4 scaleMatrix = glm::scale(scale);

        const glm::mat4 translate =
           glm::translate( biasScalar *
               glm::vec3{ position.x+ offsetX +
                          _alphabet.at(c).localTranslate.x *
                          scale.x,
                           position.y -
                          (1.f-_alphabet.at(c).localTranslate.y) * scale.y,
                          0.f}
                         );
        _charactersTransforms[label].push_back(
                    biasMatrix * translate * scaleMatrix
                    );

        offsetX += pitch;
    }
}*/

void TextRendering::updateCharacters(const Label& label)
{
    constexpr FT_UInt height = 100;
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

    /*std::vector<unsigned char> characters;
    for( const CstPage_sptr& page : pages) {
        if (page){
            for( const CstLabel_sptr& label : page->labels()) {
                if (const CstPage_sptr& childPage = page->child(label)) {

                    const std::vector<unsigned char> childCharacters =
                            getCharacters({childPage});
                    characters.insert(characters.end(), childCharacters.begin(),
                                      childCharacters.end());
                }
                if (label->typeOfLabel() == Label::TypeOfLabel::Message) {
                    for (const char& c : label->message()) {
                        characters.push_back(c);
                    }
                }
            }
        }
    }

    return characters;
}*/

/*unsigned int TextRendering::fontHeight() const {
    return _fontHeight;
}*/

FT_Library TextRendering::ftLib;
FT_Face TextRendering::fontFace;

std::map<unsigned char, TextRendering::Character> TextRendering::alphabet{};

const std::string TextRendering::vsshaderFont = "shaders/fontVs.vs";
const std::string TextRendering::fsshaderFont = "shaders/fontFs.fs";

