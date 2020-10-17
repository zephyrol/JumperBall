/*
 * File:   StarMesh.h
 * Author: Morgenthaler S
 *
 * Created on 20 septembre 2020, 11h20
 */
#include "StarMesh.h"

StarMesh::StarMesh(const Star& star):
    Mesh<Star,StarState>(star)
{

}

void StarMesh::updateWorld()
{
    setWorld(_frame.transform());
}
