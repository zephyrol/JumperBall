/*
 * File: BallMesh.h
 * Author: Morgenthaler S
 *
 * Created on 20 septembre 2020, 11h20
 */

#ifndef BALLMESH_H
#define BALLMESH_H
#include "scene/BallState.h"
#include "Mesh.h"

class BallMesh:public Mesh <Ball, BallState> {
public:
BallMesh(const Ball& ball);

private:
virtual void updateWorld() override;

};

#endif // BALLMESH_H
