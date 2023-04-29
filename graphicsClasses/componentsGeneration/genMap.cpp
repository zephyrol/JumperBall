/*
 * File: genMap.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "MeshGenerator.h"

vecMesh_sptr MeshGenerator::genBlocks(const CstMap_sptr &map) {

    vecMesh_sptr meshes;
    for (const auto &block: map->getBlocks()) {
        meshes.push_back(genBlock(map, block));
    }
    return meshes;
}

vecMesh_sptr MeshGenerator::genItems(const CstMap_sptr &map) {

    vecMesh_sptr meshes;
    for (const auto &block: map->getBlocks()) {
        for (const auto &item: block->getItems()) {
            auto itemMeshes = MeshGenerator::genMeshes(item);
            meshes.insert(
                meshes.end(),
                std::make_move_iterator(itemMeshes.begin()),
                std::make_move_iterator(itemMeshes.end())
            );
        }
    }
    return meshes;
}

vecMesh_sptr MeshGenerator::genEnemies(const CstMap_sptr &map) {

    vecMesh_sptr meshes;
    for (const auto &block: map->getBlocks()) {
        for (const auto &enemy: block->getEnemies()) {
            auto enemyMeshes = MeshGenerator::genMeshes(enemy);
            meshes.insert(
                meshes.end(),
                std::make_move_iterator(enemyMeshes.begin()),
                std::make_move_iterator(enemyMeshes.end())
            );
        }
    }
    return meshes;
}

vecMesh_sptr MeshGenerator::genSpecials(const CstMap_sptr &map) {

    vecMesh_sptr meshes;
    for (const auto &block: map->getBlocks()) {
        for (const auto &special: block->getSpecials()) {
            auto specialMeshes = MeshGenerator::genMeshes(special);
            meshes.insert(
                meshes.end(),
                std::make_move_iterator(specialMeshes.begin()),
                std::make_move_iterator(specialMeshes.end())
            );
        }
    }
    return meshes;
}
