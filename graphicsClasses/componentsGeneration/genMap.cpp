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

vecMesh_sptr MeshGenerator::genObjects (const Map& map) {

    vecMesh_sptr meshes;
    const auto blockInfos = map.blocksInfo();
    for (size_t i = 0; i < blockInfos.size(); ++i) {
        const size_t index = blockInfos.at(i).index;
        for (size_t j = 0; j < Block::objectsNumber; ++j) {
            const std::shared_ptr <const Block> block = map.getBlock(index);
            const std::shared_ptr <const Object> object = block->objects().at(j);
            if (object) {
                meshes.push_back(genObject(*object));
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
