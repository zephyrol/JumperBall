//
// Created by seb on 14/08/2021.
//

#include "MeshGenerator.h"

CstGeometricShape_sptr MeshGenerator::createGeometricShape(const Shape& shape) {

    const glm::mat4 rotation = Utility::rotationUpToDir(shape.dir());
    const glm::mat4 translation = glm::translate(Utility::convertToOpenGLFormat(shape.position()));
    const glm::mat4 scale = glm::scale(Utility::convertToOpenGLFormat(shape.scale()));
    const glm::mat4 localTransform = rotation * translation * scale;
    if(shape.aspect() == Shape::Aspect::Cylinder) {
    }

    return std::make_shared<Cube>();
}

