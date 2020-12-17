/*
 * File:   genMap.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "scene/mesh/MeshGenerator.h"

vecMeshComponent_sptr MeshGenerator::genComponents(const MapState& map) {

    vecMeshComponent_sptr components;
    const auto blockInfos = map.blocksInfo();
    for (size_t i = 0; i < blockInfos.size(); ++i) {
        vecMeshComponent_sptr blockComponents =
                genBlock(map, blockInfos.at(i).index);
        for(MeshComponent_sptr m : blockComponents) {
            components.push_back(std::move(m));
        }
    }

    for ( const std::shared_ptr<EnemyState>& EnemyState :
            map.enemiesStates()) {
        vecMeshComponent_sptr enemyComponents = genEnemy(*EnemyState);
        for(MeshComponent_sptr m : enemyComponents) {
            components.push_back(std::move(m));
        }
    }

    std::cout << "trying to gen this bitch" << std::endl;
    for ( const std::shared_ptr<SpecialState>& SpecialState:
            map.specialStates()) {
        vecMeshComponent_sptr specialComponents = genSpecial(*SpecialState);
        for(MeshComponent_sptr m : specialComponents) {
            components.push_back(std::move(m));
        }
    }

    //We have better performances if the components are stored with objects
    //with the same VAO/VBO, we will not have to change the binding later
    vecMeshComponent_sptr sortedComponents = sortComponents(components);
    return sortedComponents;
}
