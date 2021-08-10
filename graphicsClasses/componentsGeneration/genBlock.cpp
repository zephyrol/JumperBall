/*
 * File: genBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "MeshGenerator.h"


Mesh_sptr MeshGenerator::genBlock (const CstMap_sptr& map, const CstBlock_sptr& block) {
    const auto& position = block->position();
    const glm::vec3 glmPosition { position.at(0), position.at(1), position.at(2) };
    const glm::mat4 translation = glm::translate(glmPosition);

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
        const JBTypes::vec3ui& neighborPosition = positions.at(i);
        // true = closed
        if (block->mayDisappear()) {
            boolSidesInfo.at(i) = true;
        } else {
            const auto neighborBlock = map->getBlock(neighborPosition);
            boolSidesInfo.at(i) = !neighborBlock || neighborBlock->mayDisappear();
        }

    }

    const CstGeometricShape_sptr blockShape = std::make_shared <const Cube>(
        block->getColor(),
        translation,
        glm::mat4(1.f),
        boolSidesInfo
    );

    const vecCstGeometricShape_sptr sharpsShapes = genSharps(*block, blockType, glmPosition);
    const vecCstGeometricShape_sptr jumpersShapes = genJumpers(*block, blockType, glmPosition);

    vecCstGeometricShape_sptr geometricShapes {};
    geometricShapes.push_back(blockShape);
    geometricShapes.insert(geometricShapes.end(), jumpersShapes.begin(), jumpersShapes.end());
    geometricShapes.insert(geometricShapes.end(), sharpsShapes.begin(), sharpsShapes.end());
    return { std::make_shared <Mesh>( block, std::move(geometricShapes)) };
}
