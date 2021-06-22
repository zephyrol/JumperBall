/*
 * File: genSpecial.cpp
 * Author: Morgenthaler S
 *
 * Created on 15 decembre 2020, 22h02
 */

#include "MeshGenerator.h"

Mesh_sptr MeshGenerator::genSpecial (const Special& special, const Map::SpecialTypes& category) {
    vecCstGeometricShape_sptr geometricShapes;

    const std::function <std::pair <glm::vec3, glm::vec3>(const JBTypes::Color&)>
    getColors = [] (const JBTypes::Color& color) {
                    if (color == JBTypes::Color::Blue) {
                        return std::pair <glm::vec3, glm::vec3> {
                            glm::vec3(0.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 1.f)
                        };
                    } else if (color == JBTypes::Color::Green) {
                        return std::pair <glm::vec3, glm::vec3> {
                            glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.5f, 0.f)
                        };
                    } else if (color == JBTypes::Color::Red) {
                        return std::pair <glm::vec3, glm::vec3> {
                            glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.5f, 0.f, 0.f)
                        };
                    } else if (color == JBTypes::Color::Yellow) {
                        return std::pair <glm::vec3, glm::vec3> {
                            glm::vec3(1.f, 1.f, 0.f), glm::vec3(0.5f, 0.5f, 0.f)
                        };
                    } else {
                        return std::pair <glm::vec3, glm::vec3> {
                            glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f) };
                    }
                };
    const JBTypes::Color& color = special.getColor();
    const std::pair <glm::vec3, glm::vec3> colors = getColors(color);

    constexpr size_t teleporterNbOfVertices = 5;
    constexpr size_t buttonNbOfVertices = Cylinder::defaultMeriCount;

    const size_t nbOfVertices = category == Map::SpecialTypes::SwitchButton
                                ? buttonNbOfVertices
                                : teleporterNbOfVertices;

    const glm::mat4 scaleLocal = category == Map::SpecialTypes::SwitchButton
                                 ? glm::scale(glm::vec3(0.2f, 0.05f, 0.2f))
                                 : glm::scale(glm::vec3(0.7f, 0.05f, 0.7f));

    const glm::mat4& modelTranf = scaleLocal;
    geometricShapes.push_back(std::make_shared <const Cylinder>(
                                  colors.first,
                                  colors.second,
                                  nbOfVertices,
                                  modelTranf
                                  ));

    return std::make_shared <Mesh>(
        Frames <ObjectState>::genFrames <Special, SpecialState>(special),
        std::move(geometricShapes)
        );
}
