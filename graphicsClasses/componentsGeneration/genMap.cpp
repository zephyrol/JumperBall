/*
 * File: genMap.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "MeshGenerator.h"


MeshDynamicGroup_uptr MeshGenerator::genMeshDynamicGroup(
    const vecCstDisplayable_sptr &displayableVector,
    std::unique_ptr<std::function<CstMesh_sptr(
        const CstDisplayable_sptr &, short
    )>> customMeshGenerationFunction
) {

    // 1. Create map to store displayable with the same dynamic (uniforms) hash
    std::unordered_map<std::string, vecCstDisplayable_sptr> dynamicGroupHashToDisplayableList;
    for (const auto &displayable: displayableVector) {
        const auto hash = displayable->getDynamicGroupHash();
        dynamicGroupHashToDisplayableList[hash].emplace_back(displayable);
    }

    // 2. Create the meshes. Each mesh with the same hash share the same dynamicId.
    std::vector<vecCstMesh_sptr> outputMeshes;
    short dynamicsId = 0;
    for (const auto &hashDisplayableList: dynamicGroupHashToDisplayableList) {
        const auto &displayableList = hashDisplayableList.second;
        vecCstMesh_sptr meshes{};
        for (const auto &displayable: displayableList) {
            meshes.emplace_back(
                customMeshGenerationFunction == nullptr
                ? MeshGenerator::genMesh(displayable, dynamicsId)
                : (*customMeshGenerationFunction)(displayable, dynamicsId)
            );
        }
        outputMeshes.emplace_back(std::move(meshes));
        ++dynamicsId;
    }

    return std::unique_ptr<MeshDynamicGroup>(new MeshDynamicGroup(std::move(outputMeshes)));
}

CstMesh_sptr MeshGenerator::genMesh(const CstDisplayable_sptr &displayable, short dynamicsId) {
    vecCstGeometricShape_sptr geometricShapes;
    for (const auto &shape: displayable->getShapes()) {
        geometricShapes.emplace_back(createGeometricShape(shape));
    }
    return std::make_shared<const Mesh>(displayable, std::move(geometricShapes), dynamicsId);
}


MeshDynamicGroup_uptr MeshGenerator::genBlocks(const CstMap_sptr &map) {

    vecCstDisplayable_sptr displayableList;
    std::map<CstDisplayable_sptr, std::function<CstMesh_sptr(short)>> displayableToMesh;
    for (const auto &block: map->getBlocks()) {
        displayableList.emplace_back(block);
        displayableToMesh[block] = [map, block](short dynamicsId) {
            return MeshGenerator::genBlock(map, block, dynamicsId);
        };
    }

    return MeshGenerator::genMeshDynamicGroup(
        displayableList,
        std::unique_ptr<std::function<CstMesh_sptr(
            const CstDisplayable_sptr &, short
        )>>(new std::function<CstMesh_sptr(const CstDisplayable_sptr &, short)>(
            [&displayableToMesh](const CstDisplayable_sptr &displayable, short dynamicsId) {
                return displayableToMesh[displayable](dynamicsId);
            }
        ))
    );
}

MeshDynamicGroup_uptr MeshGenerator::genItems(const CstMap_sptr &map) {

    vecCstDisplayable_sptr displayableList;
    for (const auto &block: map->getBlocks()) {
        auto items = block->getItems();
        displayableList.insert(
            displayableList.end(),
            items.begin(),
            items.end()
        );
    }
    if (displayableList.empty()) {
        return nullptr;
    }
    return MeshGenerator::genMeshDynamicGroup(displayableList);
}

MeshDynamicGroup_uptr MeshGenerator::genEnemies(const CstMap_sptr &map) {

    vecCstDisplayable_sptr displayableList;
    for (const auto &block: map->getBlocks()) {
        auto enemies = block->getEnemies();
        displayableList.insert(
            displayableList.end(),
            std::make_move_iterator(enemies.begin()),
            std::make_move_iterator(enemies.end())
        );
    }
    if (displayableList.empty()) {
        return nullptr;
    }
    return MeshGenerator::genMeshDynamicGroup(displayableList);
}

MeshDynamicGroup_uptr MeshGenerator::genSpecials(const CstMap_sptr &map) {

    vecCstDisplayable_sptr displayableList;
    for (const auto &block: map->getBlocks()) {
        auto specials = block->getSpecials();
        displayableList.insert(
            displayableList.end(),
            std::make_move_iterator(specials.begin()),
            std::make_move_iterator(specials.end())
        );
    }
    if (displayableList.empty()) {
        return nullptr;
    }
    return MeshGenerator::genMeshDynamicGroup(displayableList);
}
