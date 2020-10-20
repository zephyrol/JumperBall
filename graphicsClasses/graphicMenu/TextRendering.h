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
#include <system/ParallelTask.h>
#include "LabelRendering.h"


class TextRendering: public LabelRendering {
public:
    TextRendering                       ( const Label& label, float maxHeight,
                                          const ShaderProgram& spFont);
    virtual ~TextRendering() = default;
    
    static bool                         initFreeTypeAndFont();
    static void                         clearFreeTypeRessources();

    struct Character                    {   GLuint texture;
                                            glm::vec2 localScale;
                                            glm::vec2 localTranslate; };

    void                                render() const override;
    void                                update(float offset) override;
    const ShaderProgram&                getShaderProgram() const override;
    GLuint                              getQuadVAO() const override;
    const Quad&                         getDisplayQuad() const override;

private:
    const ShaderProgram& _spFont;
    std::vector<glm::mat4> _charactersTransforms;
    static std::map<unsigned char, Character> alphabet;

    static void                         updateCharacters(const Label& label, float maxHeight);
    static void                         updateQuad();
    static FT_Library                   ftLib;
    static FT_Face                      fontFace;
    static std::shared_ptr<const Quad>  displayQuad;
    static const glm::vec3              enabledLetterColor;
    static const glm::vec3              disabledLetterColor;
};

#endif // TEXTRENDERING_H
