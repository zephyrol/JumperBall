/*
 * File: genBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "MeshGenerator.h"


Mesh_sptr MeshGenerator::genBlock (const Map& map, size_t index) {
    const JBTypes::vec3ui position = map.getBlockCoords(index);
    const glm::vec3 glmPosition { position.at(0), position.at(1), position.at(2) };
    const glm::mat4 translation = glm::translate(glmPosition);
    const Map::BlockTypes blockType = map.getType(position);

    const std::shared_ptr <const Block> block = map.getBlock(index);

    CstGeometricShape_sptr blockShape;

    constexpr size_t numberOfFaces = 6;

    // std::string strSidesInfo {};
    std::array <bool, numberOfFaces> boolSidesInfo {}; // true <=> close, false <=> open
    const std::array <bool, numberOfFaces> boolClosedSides { true, true, true, true, true, true };
    const std::array <JBTypes::vec3ui, numberOfFaces> positions {{
        { position.at(0), position.at(1), position.at(2) - 1 },
        { position.at(0), position.at(1), position.at(2) + 1 },
        { position.at(0) + 1, position.at(1), position.at(2) },
        { position.at(0) - 1, position.at(1), position.at(2) },
        { position.at(0), position.at(1) + 1, position.at(2) },
        { position.at(0), position.at(1) - 1, position.at(2) }
    }};

    for (size_t i = 0; i < numberOfFaces; ++i) {
        const JBTypes::vec3ui& neighbourgPosition = positions.at(i);
        const Map::BlockTypes typeNeighbourg = map.getType(neighbourgPosition);
        if (typeNeighbourg != Map::BlockTypes::Brittle && typeNeighbourg != Map::BlockTypes::None) {
            // strSidesInfo.push_back('0');
            boolSidesInfo.at(i) = false;
        } else {
            // strSidesInfo.push_back('1');
            boolSidesInfo.at(i) = true;
        }
    }

    if (blockType == Map::BlockTypes::Ice) {
        std::vector <glm::vec3> iceColors = Cube::iceColorsCube;
        blockShape = std::make_shared <const Cube>(
            std::move(iceColors),
            translation,
            glm::mat4(1.f),
            boolSidesInfo
            );
    } else if (blockType == Map::BlockTypes::Fire) {
        std::vector <glm::vec3> fireColors = Cube::fireColorsCube;
        blockShape = std::make_shared <const Cube>(
            std::move(fireColors),
            translation,
            glm::mat4(1.f),
            boolSidesInfo
            );
    } else if (blockType == Map::BlockTypes::Brittle) {
        blockShape = std::make_shared <const Cube>(translation);
    } else if (blockType == Map::BlockTypes::Ghost) {
        std::vector <glm::vec3> ghostColors = Cube::ghostColorsCube;
        blockShape = std::make_shared <const Cube>(
            std::move(ghostColors),
            translation,
            glm::mat4(1.f),
            boolClosedSides
            );
    } else {
        blockShape = std::make_shared <const Cube>(translation, glm::mat4(1.f), boolSidesInfo);
    }


    const vecCstGeometricShape_sptr sharpsShapes = genSharps(*block, blockType, glmPosition);
    const vecCstGeometricShape_sptr jumpersShapes = genJumpers(*block, blockType, glmPosition);

    vecCstGeometricShape_sptr geometricShapes {};
    geometricShapes.push_back(blockShape);
    geometricShapes.insert(geometricShapes.end(), jumpersShapes.begin(), jumpersShapes.end());
    geometricShapes.insert(geometricShapes.end(), sharpsShapes.begin(), sharpsShapes.end());
    return { std::make_shared <Mesh>(
                 std::unique_ptr <State>(new BlockState(*block)),
                 std::move(geometricShapes)
                 ) };
}
