//
// Created by S.Morgenthaler on 23/08/2023.
//

#ifndef JUMPERBALLAPPLICATION_MAPGROUPGENERATOR_H
#define JUMPERBALLAPPLICATION_MAPGROUPGENERATOR_H

#include "RenderGroupGenerator.h"
#include "scene/Map.h"


class MapGroupGenerator : public RenderGroupGenerator {

public:
    explicit MapGroupGenerator(CstMap_sptr map, unsigned int ballSkin);

    RenderGroup_sptr genRenderGroup() const override;

private:
    const CstMap_sptr _map;
    const unsigned int _ballSkin;

    Mesh_sptr genBlock(const CstBlock_sptr &block, short dynamicsId) const;

    Mesh_sptr genBall(const CstBall_sptr &ball, short dynamicsId) const;

    static Mesh_sptr genMesh(const CstDisplayable_sptr &displayable, short dynamicsId);

    static CstGeometricShape_sptr createGeometricShape(const CstShape_sptr &shape);
};


#endif //JUMPERBALLAPPLICATION_MAPGROUPGENERATOR_H
