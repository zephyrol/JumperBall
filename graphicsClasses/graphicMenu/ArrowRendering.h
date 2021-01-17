/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ArrowRendering.h
 * Author: Morgenthaler S
 * 
 * Created on 17 janvier 2021, 11:20
 */

#ifndef ARROWRENDERING_H
#define ARROWRENDERING_H
#include <map>
#include <gameMenu/ArrowLabel.h>
#include <system/ParallelTask.h>
#include <geometry/Triangle.h>
#include "LabelRendering.h"

class ArrowRendering;
using ArrowRendering_sptr = std::shared_ptr<ArrowRendering>;
using CstArrowRendering_sptr = std::shared_ptr<const ArrowRendering>;
using vecCstArrowRendering_sptr = std::vector<CstArrowRendering_sptr>;
using vecArrowRendering_sptr = std::vector<ArrowRendering_sptr>;

class ArrowRendering: public LabelRendering {
public:
    ArrowRendering                       (const Label& label,
                                          const ShaderProgram& spArrow);
    virtual ~ArrowRendering() = default;
    struct CharacterTransform           { glm::vec2 localScale;
                                          glm::vec2 localTranslate; };

    void                                render() const override;
    void                                update(float offset) override;
    const ShaderProgram&                getShaderProgram() const override;

    std::vector<size_t>                 getIndicesWithID(GLuint characterId)
                                                                          const;
    const glm::vec3&                    getArrowColor() const;
    
private:
    const ShaderProgram&                _spArrow;
    glm::mat4                           _transformQuad;
    glm::mat4                           _transformTriangle;
    
    void                                fillArrowureIDs();
    static void                         updateGeometry();
    static std::shared_ptr<const Quad>  displayQuad;
    static std::shared_ptr<const Triangle> displayTriangle;
    static const glm::vec3              enabledLetterColor;
    static const glm::vec3              disabledLetterColor;
};

#endif // ARROWRENDERING_H
