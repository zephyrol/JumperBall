/*
 * File:   StarMesh.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 septembre 2020, 11h20
 */
#ifndef STARMESH_H
#define STARMESH_H
#include "../GraphicStar.h"
#include "Mesh.h"

class StarMesh: public Mesh<Star,GraphicStar>
{
public:
    StarMesh(const Star& star);

private:
    virtual void updateWorld() override;
};

#endif // STARMESH_H
