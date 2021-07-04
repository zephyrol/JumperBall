/*
 * File: genQuadLabel.cpp
 * Author: Morgenthaler S
 */

#include "MeshGenerator.h"

// TODO: add custom color
Mesh_sptr MeshGenerator::genLetterLabel (
    const CstPage_sptr& page,
    const JBTypes::vec2f& position,
    float width,
    float height,
    const glm::vec3& color) {

    const glm::mat4 translation = glm::translate(glm::vec3(Utility::convertToOpenGLFormat(position), 0.f));
    const glm::mat4 scale = glm::scale(glm::vec3(width, height, 1.f));
    const std::shared_ptr <const Quad> quad = std::make_shared <const Quad>(color, translation * scale);

    vecCstGeometricShape_sptr geometricShapes { quad };
    return std::make_shared <Mesh>(page, std::move(geometricShapes));
}
