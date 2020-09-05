/*
 * File:   genMap.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "../MeshGenerator.h"

VecMeshComponentSptr MeshGenerator::genComponents(const Map& map) {

    VecMeshComponentSptr components;
    const auto blockInfos = map.blocksInfo();
    for (unsigned int i = 0 ; i < blockInfos.size(); ++i) {
        VecMeshComponentSptr blockComponents =
                genBlock(map, blockInfos.at(i).index);
        for(MeshComponentSptr m : blockComponents) {
            components.push_back(std::move(m));
        }
    }

    //We have better performances if the components are stored with objects
    //with the same VAO/VBO, we will not have to change the binding later
    VecMeshComponentSptr sortedComponents = sortComponents(components);
    return sortedComponents;
}
