/*
 * File:   MapMesh.h
 * Author: Morgenthaler S
 *
 * Created on 20 septembre 2020, 11h20
 */
#ifndef MAPMESH_H
#define MAPMESH_H
#include <Map.h>
#include "../GraphicMap.h"
#include "Mesh.h"


class MapMesh : public Mesh<Map,GraphicMap>
{
public:
    MapMesh(const Map& map);

private:
    virtual void updateWorld() override;
};

#endif // MAPMESH_H
