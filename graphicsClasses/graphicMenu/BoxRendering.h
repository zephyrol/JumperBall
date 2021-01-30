/*
 * File: BoxRendering.cpp
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


class BoxRendering;
using BoxRendering_sptr = std::shared_ptr <BoxRendering>;
using CstBoxRendering_sptr = std::shared_ptr <const BoxRendering>;
using vecCstBoxRendering_sptr = std::vector <CstBoxRendering_sptr>;
using vecBoxRendering_sptr = std::vector <BoxRendering_sptr>;

class BoxRendering:public LabelRendering {
public:
BoxRendering(const Label& label,
             const glm::vec3& color1 = glm::vec3(0.f, 0.f, 0.f),
             const glm::vec3& color2 = glm::vec3(0.f, 0.f, 0.f));

void render() const override;
void update(float offset) override;

private:
const Quad _boxQuad;
glm::mat4 _transform;

static const std::string vsshaderBox;
static const std::string fsshaderBox;
static std::shared_ptr <const ShaderProgram> spBox;
};

#endif // BOXRENDERING_H
