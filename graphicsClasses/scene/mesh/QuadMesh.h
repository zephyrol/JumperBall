/*
 * File:   QuadMesh.h
 * Author: Morgenthaler S
 *
 * Created on 20 septembre 2020, 11h20
 */
#ifndef QUADMESH_H
#define QUADMESH_H
#include "../GraphicQuad.h"
#include "Mesh.h"

class QuadMesh: public Mesh<Quad,GraphicQuad>
{
public:
    QuadMesh(const Quad& quad);

private:
    virtual void updateWorld() override;
};

#endif // QUADMESH_H
