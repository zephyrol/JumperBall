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
#include "mesh/MeshGenerator.h"

class Mesh;
using Mesh_sptr = std::shared_ptr <Mesh>;
using CstMesh_sptr = std::shared_ptr <const Mesh>;
using vecCstMesh_sptr = std::vector <Mesh_sptr>;
using vecMesh_sptr = std::vector <Mesh_sptr>;

class Mesh {
public:
Mesh(const State& state, const vecCstGeometricShape_sptr& shapes);
struct StaticAttributes {
    std::vector <glm::vec3> positions;
    std::vector <glm::vec3> normals;
    std::vector <glm::vec3> colors;
    std::vector <glm::vec2> uvCoords;
    std::vector <GLushort> indices;
};

struct DynamicAttributes {
    std::vector <glm::vec3> positions;
    std::vector <glm::vec3> normals;
    std::vector <glm::vec3> colors;
    std::vector <glm::vec2> uvCoords;
    std::vector <GLushort> indices;
};

StaticAttributes genStaticAttributes() const;
std::vector <std::vector <GLfloat> > genDynamicAttributes() const;
static Mesh::StaticAttributes concatStaticAttributes(const Mesh::StaticAttributes& current,
                                                     const Mesh::StaticAttributes& staticAttributes);

private:

template<typename T> static void concatShapeData(std::vector <T>& current, const std::vector <T>& dataShape);

const State& _state;
const vecCstGeometricShape_sptr _shapes;

};

#endif // MESH_H
