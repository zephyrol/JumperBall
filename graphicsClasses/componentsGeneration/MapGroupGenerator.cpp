//
// Created by S.Morgenthaler on 23/08/2023.
//

#include "MapGroupGenerator.h"
#include "geometry/Cylinder.h"
#include "geometry/Sphere.h"
#include "geometry/Cube.h"
#include "geometry/Pyramid.h"

MapGroupGenerator::MapGroupGenerator(CstMap_sptr map, unsigned int ballSkin) :
    _map(std::move(map)),
    _ballSkin(ballSkin) {
}

Mesh_sptr MapGroupGenerator::genMesh(const CstDisplayable_sptr &displayable, short dynamicsId) {
    vecCstGeometricShape_sptr geometricShapes;
    for (const auto &shape: displayable->getShapes()) {
        geometricShapes.emplace_back(createGeometricShape(shape));
    }
    return std::make_shared<Mesh>(displayable, std::move(geometricShapes), dynamicsId);
}

Mesh_sptr MapGroupGenerator::genBlock(const CstBlock_sptr &block, short dynamicsId) const {

    const auto &position = block->position();
    constexpr size_t numberOfFaces = 6;

    std::array<bool, numberOfFaces> boolSidesInfo{}; // true <=> close, false <=> open
    const std::array<glm::u32vec3, numberOfFaces> positions{
        {
            {position.x, position.y, position.z - 1},
            {position.x, position.y, position.z + 1},
            {position.x + 1, position.y, position.z},
            {position.x - 1, position.y, position.z},
            {position.x, position.y + 1, position.z},
            {position.x, position.y - 1, position.z}
        }
    };

    for (size_t i = 0; i < numberOfFaces; ++i) {
        const glm::u32vec3 &neighborPosition = positions.at(i);
        // true = closed
        if (block->mayDisappear()) {
            boolSidesInfo.at(i) = true;
        } else {
            const auto neighborBlock = _map->getBlock(neighborPosition);
            boolSidesInfo.at(i) = !neighborBlock || neighborBlock->mayDisappear();
        }

    }

    const auto getTranslation = [&block, &position]() {
        // Blocks that may disappear can't be gathered with others, they are independent.
        if (block->mayDisappear()) {
            return glm::mat4(1.f);
        }
        const glm::vec3 glmPosition{position.x, position.y, position.z};
        const glm::mat4 translation = glm::translate(glmPosition);
        return translation;
    };

    const CstGeometricShape_sptr blockShape = std::make_shared<const Cube>(
        block->getColor(),
        getTranslation(),
        glm::mat4(1.f),
        boolSidesInfo
    );

    vecCstGeometricShape_sptr geometricShapes{};
    geometricShapes.push_back(blockShape);
    for (const auto &shape: block->getExtraShapes()) {
        geometricShapes.push_back(createGeometricShape(shape));
    }
    return {std::make_shared<Mesh>(block, std::move(geometricShapes), dynamicsId)};
}

Mesh_sptr MapGroupGenerator::genBall(const CstBall_sptr &ball, short dynamicsId) const {
    const auto getCustomColors = [this]() -> std::vector<glm::vec3> {
        if (_ballSkin == 0) {
            const glm::vec3 color1{32.0 / 255.0, 223.0 / 255.0, 123.0 / 255.0};
            return {color1, 1.f - color1};
        }
        if (_ballSkin == 1) {
            const glm::vec3 color1{44.0 / 255.0, 211.0 / 255.0, 208.0 / 255.0};
            return {color1, 1.f - color1};
        }
        if (_ballSkin == 2) {
            const glm::vec3 color1{4.0 / 255.0, 42.0 / 255.0, 213.0 / 255.0};
            return {color1, 1.f - color1};
        }
        if (_ballSkin == 3) {
            return {
                {198.0 / 255.0, 218.0 / 255.0, 37.0 / 255.0},
                {37.0 / 255.0,  198.0 / 255.0, 218.0 / 255.0},
                {218.0 / 255.0, 37.0 / 255.0,  198.0 / 255.0}
            };
        }
        if (_ballSkin == 4) {
            return {
                {223.0 / 255.0, 142.0 / 255.0, 32.0 / 255.0},
                {32.0 / 255.0,  223.0 / 255.0, 142.0 / 255.0},
                {142.0 / 255.0, 32.0 / 255.0,  223.0 / 255.0}
            };
        }
        const glm::vec3 tetradricFirstColor{34.0 / 255.0, 221.0 / 255.0, 213.0 / 255.0};
        const glm::vec3 tetradricSecondColor{119.0 / 255.0, 34.0 / 255.0, 221.0 / 255.0};
        return {
            tetradricFirstColor,
            tetradricSecondColor,
            1.f - tetradricFirstColor,
            1.f - tetradricSecondColor
        };
    };

    const CstGeometricShape_sptr sphere = std::make_shared<const Sphere>(getCustomColors());
    vecCstGeometricShape_sptr geometricShapes{sphere};
    return std::make_shared<Mesh>(ball, std::move(geometricShapes), dynamicsId);
}

CstGeometricShape_sptr MapGroupGenerator::createGeometricShape(const CstShape_sptr &shape) {

    const auto getRotationMatrix = [](const glm::vec3 &rotation) {
        return glm::rotate(rotation.x, glm::vec3(1.f, 0.f, 0.f))
               * glm::rotate(rotation.y, glm::vec3(0.f, 1.f, 0.f))
               * glm::rotate(rotation.z, glm::vec3(0.f, 0.f, 1.f));
    };

    const auto getTranslationMatrix = [](const glm::vec3 &translation) {
        return glm::translate(translation);
    };

    const auto getScaleMatrix = [](const glm::vec3 &scale) {
        return glm::scale(scale);
    };

    const std::map<
        Transformation::Type,
        std::function<glm::mat4(const glm::vec3 &)>
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

    for (const auto &transformation: transformations) {
        const auto transformMat = transformationFunctions.at(transformation.type())(transformation.value());

        localTransform = transformMat * localTransform;
        if (transformation.type() == Transformation::Type::Rotation) {
            normalTransform = transformMat * normalTransform;
        }

    }

    if (shape->aspect() == Shape::Aspect::Cylinder) {
        return std::make_shared<Cylinder>(shape->color(), 60, localTransform, normalTransform);
    }
    if (shape->aspect() == Shape::Aspect::Pyramid) {
        return std::make_shared<Pyramid>(localTransform, normalTransform);
    }
    if (shape->aspect() == Shape::Aspect::Cube) {
        return std::make_shared<Cube>(shape->color(), localTransform, normalTransform);
    }
    if (shape->aspect() == Shape::Aspect::Sphere) {
        return std::make_shared<Sphere>(shape->color(), localTransform, normalTransform);
    }
    if (shape->aspect() == Shape::Aspect::Pedestal) {
        return std::make_shared<Cylinder>(shape->color(), 5, localTransform, normalTransform);
    }
    std::cerr << "Shape aspect not managed" << std::endl;
    return nullptr;
}

RenderGroup_sptr MapGroupGenerator::genRenderGroup() const {

    // 1. Create map to store displayable with its function to generate its representing mesh
    std::unordered_map<CstDisplayable_sptr, std::function<Mesh_sptr(short)>> displayableToMeshGeneration{
        {_map->getBall(), [this](short dynamicsId) { return genBall(_map->getBall(), dynamicsId); }}
    };

    for (const auto &block: _map->getBlocks()) {
        displayableToMeshGeneration[block] = [this, block](short dynamicsId) {
            return genBlock(block, dynamicsId);
        };

        vecCstDisplayable_sptr elementsOnBlocks;
        auto items = block->getItems();
        elementsOnBlocks.insert(
            elementsOnBlocks.end(),
            std::make_move_iterator(items.begin()),
            std::make_move_iterator(items.end())
        );
        auto enemies = block->getEnemies();
        elementsOnBlocks.insert(
            elementsOnBlocks.end(),
            std::make_move_iterator(enemies.begin()),
            std::make_move_iterator(enemies.end())
        );
        auto specials = block->getSpecials();
        elementsOnBlocks.insert(
            elementsOnBlocks.end(),
            std::make_move_iterator(specials.begin()),
            std::make_move_iterator(specials.end())
        );
        for (const auto &displayable: elementsOnBlocks) {
            displayableToMeshGeneration[displayable] = [displayable](short dynamicsId) {
                return genMesh(displayable, dynamicsId);
            };
        }
    }

    // 2. Create map to store displayable with the same dynamic (uniforms) hash
    std::unordered_map<std::string, vecCstDisplayable_sptr> dynamicGroupHashToDisplayableList;
    for (const auto &displayableFunction: displayableToMeshGeneration) {
        const auto &displayable = displayableFunction.first;
        const auto hash = displayable->getDynamicGroupHash();
        dynamicGroupHashToDisplayableList[hash].emplace_back(displayable);
    }

    // 3. Create the meshes. Each mesh with the same hash share the same dynamicId.
    std::vector<vecCstMesh_sptr> outputMeshes;
    short dynamicsId = 0;
    for (const auto &hashDisplayableList: dynamicGroupHashToDisplayableList) {
        const auto &displayableList = hashDisplayableList.second;
        vecCstMesh_sptr meshes{};
        for (const auto &displayable: displayableList) {
            meshes.emplace_back(displayableToMeshGeneration.at(displayable)(dynamicsId));
        }
        outputMeshes.emplace_back(std::move(meshes));
        ++dynamicsId;
    }

    auto meshDynamicGroup = std::unique_ptr<MeshDynamicGroup>(new MeshDynamicGroup(std::move(outputMeshes)));
    return RenderGroup::createInstance(std::move(meshDynamicGroup));
}


