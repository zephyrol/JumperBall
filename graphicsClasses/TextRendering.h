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

#ifndef TEXTRENDERING_H
#define TEXTRENDERING_H
#include <map>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <gameMenu/MessageLabel.h>
#include "Utility.h"
#include "ShaderProgram.h"
#include "geometry/Quad.h"


class TextRendering
{
public:
    TextRendering                       ( const std::vector<unsigned char>& 
                                          characters = {},
                                          unsigned int height = 0);
    
    static bool                         initFreeTypeAndFont();
    static void                         clearFreeTypeRessources();

    struct Character                    {   GLuint texture;
                                            glm::vec2 localScale;
                                            glm::vec2 localTranslate; };

    void                                render( const Label& label, 
                                                const glm::vec3& color) const;
    unsigned int                        fontHeight() const;

private:

    const std::map<unsigned char, Character>
                                        _alphabet;
    
    const unsigned int                  _fontHeight;
    const Quad                          _displayQuad;

    std::map<unsigned char, Character>  initAlphabet(
                                              const std::vector<unsigned char>& 
                                                                    characters,  
                                                          unsigned int height);
    const ShaderProgram                 _spFont;


    static FT_Library                   ftLib;
    static FT_Face                      fontFace;

    static const std::string            vsshaderFont;
    static const std::string            fsshaderFont;

};

#endif // TEXTRENDERING_H
