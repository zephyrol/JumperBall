/*
 * File: genOthersLabel.cpp
 * Author: Morgenthaler S
 *
 * Created on 30 avril 2021, 14:20
 */

#include "MeshGenerator.h"


vecMesh_sptr MeshGenerator::genOthersLabel (const CstPage_sptr& page) {

    vecCstGeometricShape_sptr geometricShapes {};
    for (const auto& label : page->labels()) {
        vecCstGeometricShape_sptr geometricShapesLabel = genGeometricShapesFromLabel(*label);
        geometricShapes.insert(
            geometricShapes.end(),
            std::make_move_iterator(geometricShapesLabel.begin()),
            std::make_move_iterator(geometricShapesLabel.end())
            );
    }
    Mesh_sptr mesh = std::make_shared <Mesh>(page, std::move(geometricShapes));
    return { mesh };
}
