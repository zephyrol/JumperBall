
/* 
 * File:   MeshGenerator.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 12 avril 2020, 19:24
 */

#include "MeshGenerator.h"
#include <algorithm>

vecMeshComponent_sptr MeshGenerator::sortComponents(
                                const vecMeshComponent_sptr& components) {
    vecMeshComponent_sptr sortedComponents;
    bool hasToSwap = true;
    std::vector<size_t> indices (components.size());
    for (unsigned int i = 0; i < components.size(); ++i) {
        indices.at(i) = i;
    }
    while(hasToSwap) {
        hasToSwap = false;
        for (size_t i = 0; i < components.size()-1; ++i) {
            const CstMeshComponent_sptr c1 = components.at(indices.at(i));
            const CstMeshComponent_sptr c2 = components.at(indices.at(i+1));
            if (*c1->getShapeVAO() > *c2->getShapeVAO() ){
                const size_t intermediate = indices.at(i);
                indices.at(i) = indices.at(i+1);
                indices.at(i+1) = intermediate;
                hasToSwap = true;
            }
        }
    }
    
    for (unsigned int i = 0; i < components.size(); ++i) {
        const size_t index = indices.at(i);
        sortedComponents.push_back(std::move(components.at(index)));
    }
    return sortedComponents;
}
std::map<std::string, std::shared_ptr<GeometricShape> > 
MeshGenerator::commonShapes;
