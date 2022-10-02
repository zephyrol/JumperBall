/*
 * File: genGeometricShapesFromLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 avril 2021, 14:30
 */
#include "MeshGenerator.h"

vecCstGeometricShape_sptr MeshGenerator::genGeometricShapesFromLabel(const Label &label) {

    const glm::vec3 color = label.color() == JBTypes::Color::Blue
            ? decltype(color)(0.f, 1.f, 1.f)
            : decltype(color)(1.f, 1.f, 1.f);

    const auto genGeometricShape =
        [&color](
            const LabelGeometry::Shape &shape,
            const glm::mat4 &model,
            const glm::mat4 &normalTransform,
            std::vector<glm::vec2>&& uvs
        ) -> CstGeometricShape_sptr {
            switch (shape) {
                case LabelGeometry::Shape::Quad:
                    return std::make_shared<const Quad>(color, model, normalTransform, std::move(uvs));
                    break;
                case LabelGeometry::Shape::Triangle:
                    return std::make_shared<const Triangle>(color, model, normalTransform);
                    break;
                default:
                    return std::make_shared<const Quad>(model, normalTransform);
                    break;
            }
            return std::make_shared<const Quad>(model, normalTransform);
        };


    vecCstGeometricShape_sptr geometricShapes;

    const vecLabelGeometry geometries = label.genGeometries();
    for (const LabelGeometry &geometry: geometries) {
        const LabelGeometry::ShapeTranslation &shapeTranslation = geometry.getTranslation();
        const LabelGeometry::ShapeRotation &shapeRotation = geometry.getRotation();
        const LabelGeometry::ShapeScale &shapeScale = geometry.getScale();

        const glm::mat4 localTranslation = glm::translate(
            glm::vec3(shapeTranslation.at(0), shapeTranslation.at(1), shapeTranslation.at(2))
        );

        const glm::mat4 localRotationX = glm::rotate(shapeRotation.at(0), glm::vec3(1.f, 0.f, 0.f));
        const glm::mat4 localRotationY = glm::rotate(shapeRotation.at(1), glm::vec3(0.f, 1.f, 0.f));
        const glm::mat4 localRotationZ = glm::rotate(shapeRotation.at(2), glm::vec3(0.f, 0.f, 1.f));
        const glm::mat4 localRotation = localRotationZ * localRotationY * localRotationX;

        const glm::mat4 localScale = glm::scale(
            glm::vec3(shapeScale.at(0), shapeScale.at(1), shapeScale.at(2))
        );

        // We apply rotation because scale
        const glm::mat4 model = localTranslation * localScale * localRotation;

        std::vector<glm::vec2> uvs {};
        for(const auto& uv: geometry.getCustomUvs()) {
            uvs.emplace_back(uv[0], uv[1]);
        }

        // TODO add scale
        const glm::mat4 normalTransform = localRotation;
        geometricShapes.push_back(genGeometricShape(
            geometry.getShape(),
            model,
            normalTransform,
            std::move(uvs)
        ));
    }

    return geometricShapes;

}
