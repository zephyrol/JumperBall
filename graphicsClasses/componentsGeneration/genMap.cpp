/*
 * File: genMap.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "MeshGenerator.h"

vecMesh_sptr MeshGenerator::genBlocks (const Map& map) {

    vecMesh_sptr meshes;
    const auto blockInfos = map.blocksInfo();
    for (size_t i = 0; i < blockInfos.size(); ++i) {
        meshes.push_back(genBlock(map, blockInfos.at(i).index));
    }
    return meshes;
}

vecMesh_sptr MeshGenerator::genItems (const Map& map) {

    vecMesh_sptr meshes;
    const auto blockInfos = map.blocksInfo();
    for (size_t i = 0; i < blockInfos.size(); ++i) {
        const size_t index = blockInfos.at(i).index;
        for (size_t j = 0; j < Block::itemsNumber; ++j) {
            const CstBlock_sptr block = map.getBlock(index);
            const std::shared_ptr <const Item> item = block->items().at(j);
            if (item) {
                meshes.push_back(genItem(*item));
            }
        }
    }
    return meshes;
}

vecMesh_sptr MeshGenerator::genEnemies (const Map& map) {

    vecMesh_sptr meshes;
    const auto enemiesInfos = map.getEnemiesInfo();
    for (const auto& enemyInfo : enemiesInfos) {
        const std::shared_ptr <const Enemy> enemy = enemyInfo.enemy;
        if (enemy) {
            meshes.push_back(genEnemy(*enemy, enemyInfo.type));
        }
    }
    return meshes;
}

vecMesh_sptr MeshGenerator::genSpecials (const Map& map) {

    vecMesh_sptr meshes;
    const auto specialsInfos = map.getSpecialInfo();
    for (const auto& specialInfo : specialsInfos) {
        const std::shared_ptr <const Special> special = specialInfo.special;
        if (special) {
            meshes.push_back(genSpecial(*special, specialInfo.type));
        }
    }
    return meshes;
}
