/*
 * File:   genBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "scene/mesh/MeshGenerator.h"

vecMeshComponent_sptr MeshGenerator::genBlock
    (const MapState& map, size_t index) {

    const std::array<unsigned int,3> position = map.getBlockCoords(index);
    const Map::BlockTypes blockType = map.getType(position);

    const std::shared_ptr<BlockState> block = map.BlockStates().at(index);

    vecMeshComponent_sptr components;
    std::shared_ptr<GeometricShape> shape;

    std::string strSidesInfo {};
    std::array<bool,6> boolSidesInfo {}; // true <=> close, false <=> open
    const std::array<std::array<unsigned int, 3>,6 > positions {{
        {position.at(0),position.at(1),position.at(2) - 1},
        {position.at(0),position.at(1),position.at(2) + 1},
        {position.at(0) + 1,position.at(1),position.at(2)},
        {position.at(0) - 1,position.at(1),position.at(2)},
        {position.at(0),position.at(1) + 1,position.at(2)},
        {position.at(0),position.at(1) - 1,position.at(2)}
    }};

    for (size_t i = 0; i < 6; ++i) {
        const std::array<unsigned int, 3>& neighbourgPosition = positions.at(i);
        const Map::BlockTypes typeNeighbourg =
                map.getType(neighbourgPosition);
        if (typeNeighbourg != Map::BlockTypes::Brittle &&
                typeNeighbourg != Map::BlockTypes::None)
        {
            strSidesInfo.push_back('0');
            boolSidesInfo.at(i) = false;
        }
        else
        {
            strSidesInfo.push_back('1');
            boolSidesInfo.at(i) = true;
        }
    }

    if (blockType == Map::BlockTypes::Ice) {
        if (commonShapes.find("iceCube" + strSidesInfo) == commonShapes.end()) {
            commonShapes["iceCube" + strSidesInfo] = std::make_shared<Cube>
                (Cube::iceColorsCube,boolSidesInfo);
        }
        shape = commonShapes.at("iceCube" + strSidesInfo);
    } else if (blockType == Map::BlockTypes::Fire) {
        if (commonShapes.find("fireCube" + strSidesInfo) ==
            commonShapes.end()) {
            commonShapes["fireCube" + strSidesInfo] = std::make_shared<Cube>
                (Cube::fireColorsCube,boolSidesInfo);
        }
        shape = commonShapes.at("fireCube" + strSidesInfo);
    } else if (blockType == Map::BlockTypes::Brittle) {
        if (commonShapes.find("britlleCube") ==
            commonShapes.end()) {
            commonShapes["brittleCube"] = std::make_shared<Cube>();
        }
        shape = commonShapes.at("brittleCube");
    } else if (blockType == Map::BlockTypes::Ghost) {
        if (commonShapes.find("ghostCube") ==
            commonShapes.end()) {
            commonShapes["ghostCube"] = std::make_shared<Cube>(
                        Cube::ghostColorsCube);
        }
        shape = commonShapes.at("ghostCube");
    }
    else {
        if (commonShapes.find("basicCube" + strSidesInfo) ==
            commonShapes.end()) {
            commonShapes["basicCube" + strSidesInfo] =
            std::make_shared<Cube>(glm::mat4(1.f),glm::mat4(1.f)
                                   ,boolSidesInfo);
        }
        shape = commonShapes.at("basicCube" + strSidesInfo);
    }

    const glm::vec3 glmPosition { position.at(0), position.at(1),
                                  position.at(2)};
    const glm::mat4 transform (glm::translate(glmPosition));

    const std::shared_ptr<BlockAnimation> blockAnim = (!block->isFixed())
            ? std::make_shared<BlockAnimation>(*block)
            : nullptr;
    const MeshComponent_sptr component = std::make_shared<MeshComponent>
        (std::make_shared<Cube> (*shape,transform), blockAnim);
    components.push_back(component);

    const vecMeshComponent_sptr sharpsComponents =
        genSharps(*block,blockType, glmPosition);
    for(MeshComponent_sptr sp : sharpsComponents) {
        components.push_back(std::move(sp));
    }

    vecMeshComponent_sptr jumperComponents =
        genJumpers(*block,blockType,glmPosition);
    for(MeshComponent_sptr m : jumperComponents) {
        components.push_back(std::move(m));
    }


    for (size_t i = 0; i < Block::objectsNumber ; ++i) {
        const std::shared_ptr<const ObjectState> object =
                block->objectStates().at(i);
        if (object) {
            const JBTypes::Dir dir =
                JBTypesMethods::integerAsDirection(
                    static_cast<unsigned int>(i));
            vecMeshComponent_sptr objMCs = genObject(*object,glmPosition,dir);
            for(MeshComponent_sptr objMC : objMCs) {
                components.push_back(std::move(objMC));
            }
        }
    }

    return components;
}
