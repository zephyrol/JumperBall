/*
 * File: genMap.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "MeshGenerator.h"

vecMesh_sptr MeshGenerator::genBlocks (const CstMap_sptr& map) {

    vecMesh_sptr meshes;
    for (const auto& block: map->getBlocks()) {
        meshes.push_back(genBlock(map, block));
    }
    return meshes;
}

vecMesh_sptr MeshGenerator::genItems (const CstMap_sptr& map) {

    vecMesh_sptr meshes;
    for (const auto& block: map->getBlocks()) {
        for (const auto& itemShapes : block->getItemShapes()) {
            const auto& item = itemShapes.first;
            const auto& shapes = itemShapes.second;
            vecCstGeometricShape_sptr geometricShapes;
            for(const auto& shape: shapes) {
                geometricShapes.push_back(createGeometricShape(shape));
            }
            const auto mesh = std::make_shared <Mesh>(item, std::move(geometricShapes));
        }
    }
    return meshes;
}

vecMesh_sptr MeshGenerator::genEnemies (const CstMap_sptr& map) {

    vecMesh_sptr meshes;
    /*const auto enemiesInfos = map->getEnemiesInfo();
    for (const auto& enemyInfo : enemiesInfos) {
        const std::shared_ptr <const Enemy> enemy = enemyInfo.enemy;
        if (enemy) {
            meshes.push_back(genEnemy(enemy, enemyInfo.type));
        }
    }*/
    return meshes;
}

vecMesh_sptr MeshGenerator::genSpecials (const CstMap_sptr& map) {

    vecMesh_sptr meshes;
    /*const auto specialsInfos = map->getSpecialInfo();
    for (const auto& specialInfo : specialsInfos) {
        const std::shared_ptr <const Special> special = specialInfo.special;
        if (special) {
            meshes.push_back(genSpecial(special, specialInfo.type));
        }
    }*/
    return meshes;
}
