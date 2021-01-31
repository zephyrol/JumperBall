/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */

#ifndef MESH_H
#define MESH_H
#include "Shader.h"
#include "ShaderProgram.h"
#include <system/ParallelTask.h>
#include <scene/Ball.h>
#include <scene/Map.h>
#include "Star.h"
#include "MeshGenerator.h"

class Mesh {
Mesh();
const State& _state;
const std::vector <std::shared_ptr <GeometricShape> > _shapes;

void update();



};

#endif // MESH_H
