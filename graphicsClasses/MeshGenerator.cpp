/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MeshGenerator.cpp
 * Author: Morgenthaler S 
 * 
 * Created on 12 avril 2020, 19:24
 */

#include "MeshGenerator.h"
#include <iterator>
#include <algorithm>



VecMeshComponentSptr MeshGenerator::sortComponents(
                                VecMeshComponentSptr& components) {
    VecMeshComponentSptr sortedComponents;
    bool hasToSwap = true;
    std::vector<size_t> indices (components.size());
    for (unsigned int i = 0; i < components.size(); ++i) {
        indices.at(i) = i;
    }
    while(hasToSwap) {
        hasToSwap = false;
        for (size_t i = 0; i < components.size()-1; ++i) {
            CstMeshComponentSptr c1 = components.at(indices.at(i));
            CstMeshComponentSptr c2 = components.at(indices.at(i+1));
            if (*c1->shape()->vertexArrayObject() >
                    *c2->shape()->vertexArrayObject() ){
                const size_t intermediate = indices.at(i);
                indices.at(i) = indices.at(i+1);
                indices.at(i+1) = intermediate;
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

