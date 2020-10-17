/*
 * File:   MapMesh.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 septembre 2020, 11h20
 */
#include "MapMesh.h"

MapMesh::MapMesh(const Map &map):
    Mesh<Map,MapState>(map) {

}

void MapMesh::updateWorld() {
}
