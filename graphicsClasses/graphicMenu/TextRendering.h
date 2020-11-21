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

class TextRendering;
using TextRendering_sptr = std::shared_ptr<TextRendering>;
using CstTextRendering_sptr = std::shared_ptr<const TextRendering>;
using vecCstTextRendering_sptr = std::vector<CstTextRendering_sptr>;
using vecTextRendering_sptr = std::vector<TextRendering_sptr>;

class TextRendering: public LabelRendering {
public:
    TextRendering                       ( const Label& label,
                                          const ShaderProgram& spFont);
    virtual ~TextRendering() = default;
    struct CharacterTransform           { glm::vec2 localScale;
                                          glm::vec2 localTranslate; };

    using AlphabetTransforms = std::map<unsigned char, CharacterTransform>;
    using AlphabetTextureKey = std::pair<unsigned char, FT_UInt>;
    using AlphabetTextures = 
        std::map< AlphabetTextureKey, GLuint>;

    void                                render() const override;
    void                                update(float offset) override;
    const ShaderProgram&                getShaderProgram() const override;
    GLuint                              getQuadVAO() const override;
    const Quad&                         getDisplayQuad() const override;

    void                                render(size_t index) const;
    std::vector<size_t>                 getIndicesWithID(GLuint characterId) 
                                                                          const;
    const glm::vec3&                    getTextColor() const;
    
    static bool                         initFreeTypeAndFont();
    static void                         clearFreeTypeRessources();
    static const std::vector<GLuint>&   getAlphabetCharactersIds();


private:
    const ShaderProgram&                _spFont;
    std::vector<glm::mat4>              _charactersTransforms;
    std::vector<GLuint>                 _charactersTextureIDs;
    void                                fillTextureIDs();

    FT_UInt                             getHeightInPixels() const;
    static FT_UInt                      getHeightInPixels(const Label& label);
    static AlphabetTextures             alphabetTextures;
    static AlphabetTransforms           alphabetTransforms;
    static std::vector<GLuint>          alphabetCharactersIds;
    static void                         updateAlphabets(const Label& label);
    static void                         updateAlphabetCharactersIds();
    static void                         updateQuad();
    static FT_Library                   ftLib;
    static FT_Face                      fontFace;
    static std::shared_ptr<const Quad>  displayQuad;
    static const glm::vec3              enabledLetterColor;
    static const glm::vec3              disabledLetterColor;
};

#endif // TEXTRENDERING_H
