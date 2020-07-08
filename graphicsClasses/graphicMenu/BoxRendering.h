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
#include "Utility.h"
#include "ShaderProgram.h"
#include "geometry/Quad.h"


class BoxRendering
{
public:
    BoxRendering                       (const glm::vec3& color1,
                                        const glm::vec3& color2);

    void                                render(const Label& label) const;
 private:
    const Quad                          _boxQuad;

    const ShaderProgram                 _spBox;

    static const std::string            vsshaderBox;
    static const std::string            fsshaderBox;

};

#endif // BOXRENDERING_H