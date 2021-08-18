//
// Created by seb on 14/08/2021.
//

#include "MeshGenerator.h"

CstGeometricShape_sptr MeshGenerator::createGeometricShape(const CstShape_sptr& shape) {

    const auto& shapeDir = shape->dir();
    const glm::mat4 localRotation = shapeDir 
        ? Utility::rotationUpToDir(*shapeDir)
        : glm::mat4(1.f);

    const glm::mat4 rotation = Utility::rotationUpToDir(shape->dir());
    const glm::mat4 translation = glm::translate(Utility::convertToOpenGLFormat(shape.position()));
    const glm::mat4 scale = glm::scale(Utility::convertToOpenGLFormat(shape.scale()));
    const glm::mat4 localTransform = rotation * translation * scale;
    if(shape.aspect() == Shape::Aspect::Cylinder) {
        return std::make_shared<Cylinder>(shape.color(), 60, localTransform, rotation);
    }
    if(shape.aspect() == Shape::Aspect::Pyramid) {
        return std::make_shared<Pyramid>(localTransform, rotation);
    }
    if(shape.aspect() == Shape::Aspect::Cube) {
        return std::make_shared<Cube>(shape.color(), localTransform, rotation);
    }
    if(shape.aspect() == Shape::Aspect::Sphere) {
        return std::make_shared<Sphere>(shape.color(), localTransform, rotation);
    }
    std::cerr << "Shape aspect not managed" << std::endl;
    return nullptr;
}

