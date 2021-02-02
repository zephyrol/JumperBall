/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */
#include "Mesh.h"

Mesh::Mesh(const State& state, const vecCstGeometricShape_sptr& shapes) :
  _state(state),
  _shapes(shapes)
{
}

template<typename T> void Mesh::concatShapeData (std::vector <T>& current,
                                                 const std::vector <T>& dataShape) {
    current.insert(current.end(), dataShape.begin(), dataShape.end());
}

Mesh::StaticAttributes Mesh::concatStaticAttributes (const Mesh::StaticAttributes& current,
                                                     const Mesh::StaticAttributes& other) {
    Mesh::StaticAttributes staticAttributes = current;
    concatShapeData(staticAttributes.positions, other.positions);
    concatShapeData(staticAttributes.normals, other.normals);
    concatShapeData(staticAttributes.colors, other.colors);
    concatShapeData(staticAttributes.uvCoords, other.uvCoords);

    std::vector <GLushort> newIndices = other.indices;
    for (GLushort& newIndice : newIndices) {
        newIndice += static_cast <GLushort>(staticAttributes.positions.size());
    }
    concatShapeData(staticAttributes.indices, other.indices);
    return staticAttributes;
}

Mesh::StaticAttributes Mesh::genStaticAttributes() const {
    Mesh::StaticAttributes staticAttributes;
    for (const CstGeometricShape_sptr& shape : _shapes) {
        concatShapeData(staticAttributes.positions, *shape->positions());
        concatShapeData(staticAttributes.normals, *shape->normals());
        concatShapeData(staticAttributes.colors, *shape->colors());
        concatShapeData(staticAttributes.uvCoords, *shape->uvCoords());
        concatShapeData(staticAttributes.indices, *shape->indices());
    }
    return staticAttributes;
}

std::vector <std::vector <GLfloat> > Mesh::genDynamicAttributes() const {
    return {};
}
