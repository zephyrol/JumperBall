/*
 * File:   genBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "../MeshGenerator.h"

std::vector<MeshComponent> MeshGenerator::genBlock
    (const Map& map, size_t index) {

    const std::array<unsigned int,3> position = map.getBlockCoords(index);
    const Map::BlockTypes blockType = map.getType(position);

    const std::shared_ptr<const Block> block = map.getBlock(index);

    std::vector<MeshComponent> components;
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
        const Map::BlockTypes typeNeighbourg = map.getType(neighbourgPosition);
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

    std::shared_ptr<BlockAnimation> blockAnim =
            std::make_shared<BlockAnimation>(*block);
    MeshComponent component
        (std::make_shared<Cube> (*shape,transform), blockAnim);
    components.push_back(component);

    std::vector<MeshComponent> sharpsComponents =
        genSharps(*block,blockType, glmPosition);
    for(MeshComponent& m : sharpsComponents) {
        components.push_back(std::move(m));
    }

    std::vector<MeshComponent> jumperComponents =
        genJumpers(*block,blockType,glmPosition);
    for(MeshComponent& m : jumperComponents) {
        components.push_back(std::move(m));
    }

    const std::array<std::shared_ptr<const Object>,6> objects =
        block->objects();

    for (size_t i = 0; i < objects.size() ; ++i) {
        if ( objects.at(i) ) {
            const JBTypes::Dir dir =
                JBTypesMethods::integerAsDirection(
                    static_cast<unsigned int>(i));
            std::vector<MeshComponent> v =
                genComponents(objects.at(i),glmPosition,dir);
            for(MeshComponent& m : v) {
                components.push_back(std::move(m));
            }
        }
    }

    return components;
}