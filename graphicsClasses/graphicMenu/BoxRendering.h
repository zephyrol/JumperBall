/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BoxRendering.cpp
 * Author: Morgenthaler S
 * 
 * Created on 14 juin 2020
 */

#ifndef BOXRENDERING_H
#define BOXRENDERING_H
#include <map>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <gameMenu/BoxLabel.h>
#include "LabelRendering.h"
#include "Utility.h"
#include "ShaderProgram.h"
#include "geometry/Quad.h"


class BoxRendering : public LabelRendering
{
public:
    BoxRendering             (const Label& label,
                              const glm::vec3& color1,
                              const glm::vec3& color2,
                              const ShaderProgram& spBox);

    void                     render() const override;
    void                     update(float offset) override;
 private:
    const Quad               _boxQuad;
    const ShaderProgram& _spBox;
    glm::mat4                _transformCharacter;


};

#endif // BOXRENDERING_H
