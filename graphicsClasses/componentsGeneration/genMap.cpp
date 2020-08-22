/*
 * File:   genMap.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "../MeshGenerator.h"

std::vector<MeshComponent> MeshGenerator::genComponents(const Map& map) {

    std::vector<MeshComponent> components;
    const auto blockInfos = map.blocksInfo();
    for (unsigned int i = 0 ; i < blockInfos.size(); ++i) {
        std::vector<MeshComponent> blockComponents =
                genBlock(map, blockInfos.at(i).index);
        for(MeshComponent& m : blockComponents) {
            components.push_back(std::move(m));
        }
    }

    //We have better performances if the components are stored with objects
    //with the same VAO/VBO, we will not have to change the binding later
    std::vector<MeshComponent> sortedComponents = sortComponents(components);
    return sortedComponents;
}