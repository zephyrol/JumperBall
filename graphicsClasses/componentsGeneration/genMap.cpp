/*
 * File: genMap.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "scene/mesh/MeshGenerator.h"

vecMesh_sptr MeshGenerator::genBlocks (const Map& map) {

    vecMesh_sptr meshes;
    const auto blockInfos = map.blocksInfo();
    for (size_t i = 0; i < blockInfos.size(); ++i) {
        meshes.push_back(genBlock(map, blockInfos.at(i).index));
    }

    /*for (
        const std::shared_ptr <EnemyState>& EnemyState : map.enemiesStates()
        ) {
        vecMeshComponent_sptr enemyComponents = genEnemy(*EnemyState);
        for (MeshComponent_sptr m : enemyComponents) {
            components.push_back(std::move(m));
        }
       }

       for (
        const std::shared_ptr <SpecialState>& SpecialState : map.specialStates()
        ) {
        vecMeshComponent_sptr specialComponents = genSpecial(*SpecialState);
        for (MeshComponent_sptr m : specialComponents) {
            components.push_back(std::move(m));
        }
       }

       // We have better performances if the components are stored with objects
       // with the same VAO/VBO, we will not have to change the binding later
       vecMeshComponent_sptr sortedComponents = sortComponents(components);*/
    // return sortedComponents;

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
