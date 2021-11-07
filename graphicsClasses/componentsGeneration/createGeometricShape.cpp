//
// Created by seb on 14/08/2021.
//

#include "MeshGenerator.h"

CstGeometricShape_sptr MeshGenerator::createGeometricShape(const CstShape_sptr& shape) {

    const auto getRotationMatrix = [](const JBTypes::vec3f& rotation){
        return glm::rotate(rotation.x, glm::vec3(1.f,0.f,0.f))
            * glm::rotate(rotation.y, glm::vec3(0.f,1.f,0.f))
            * glm::rotate(rotation.z, glm::vec3(0.f,0.f,1.f));
    };

    const auto getTranslationMatrix = [](const JBTypes::vec3f& translation){
        return glm::translate(Utility::convertToOpenGLFormat(translation));
    };

    const auto getScaleMatrix = [](const JBTypes::vec3f& scale){
        return glm::scale(Utility::convertToOpenGLFormat(scale));
    };

    const std::map<
        Transformation::Type,
        std::function<glm::mat4(const JBTypes::vec3f&)>
    > transformationFunctions = {
        {
            Transformation::Type::Translation,
            getTranslationMatrix
        },
        {
            Transformation::Type::Rotation,
            getRotationMatrix
        },
        {
            Transformation::Type::Scale,
            getScaleMatrix
        }
    };


    // Creating a copy of transformations to reverse them
    auto transformations = shape->transformations();
    //transformations.reverse();

    glm::mat4 localTransform = glm::mat4(1.f);
    glm::mat4 normalTransform = glm::mat4(1.f);

    for (const auto& transformation: transformations) {
        const auto transformMat = transformationFunctions.at(transformation.type())(transformation.value());

        localTransform = transformMat * localTransform;
        if(transformation.type() == Transformation::Type::Rotation) {
            normalTransform = transformMat * normalTransform;
        }

    }

    if(shape->aspect() == Shape::Aspect::Cylinder) {
        return std::make_shared<Cylinder>(shape->color(), 60, localTransform, normalTransform);
    }
    if(shape->aspect() == Shape::Aspect::Pyramid) {
        return std::make_shared<Pyramid>(localTransform, normalTransform);
    }
    if(shape->aspect() == Shape::Aspect::Cube) {
        return std::make_shared<Cube>(shape->color(), localTransform, normalTransform);
    }
    if(shape->aspect() == Shape::Aspect::Sphere) {
        return std::make_shared<Sphere>(shape->color(), localTransform, normalTransform);
    }
    if(shape->aspect() == Shape::Aspect::Pedestal) {
        return std::make_shared<Cylinder>(shape->color(), 5, localTransform, normalTransform);
    }
    std::cerr << "Shape aspect not managed" << std::endl;
    return nullptr;
}

