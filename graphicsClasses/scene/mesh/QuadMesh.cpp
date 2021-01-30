/*
 * File: QuadMesh.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 septembre 2020, 11h20
 */
#include "QuadMesh.h"

QuadMesh::QuadMesh(const Quad& quad):
    Mesh <Quad, QuadState>(quad) {
}

void QuadMesh::updateWorld() {

}
