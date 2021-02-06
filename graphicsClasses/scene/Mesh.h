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
#include <geometry/GeometricShape.h>
#include <scene/State.h>

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
    std::vector <std::vector <GLubyte> > dynamicUbytes;
    std::vector <std::vector <GLfloat> > dynamicFloats;
    std::vector <std::vector <glm::vec3> > dynamicsVec3s;
    std::vector <std::vector <glm::vec2> > dynamicsVec2s;
};

struct Uniforms {
    std::map <std::string, GLuint> uniformUints;
    std::map <std::string, GLfloat> uniformFloats;
    std::map <std::string, glm::vec3> uniformVec3s;
    std::map <std::string, glm::vec2> uniformVec2s;
};

void update();

size_t numberOfVertices() const;
template<typename Attributes> Attributes genAttributes() const;
// StaticAttributes genStaticAttributes() const;
// DynamicAttributes genDynamicAttributes() const;
Uniforms genUniformsValues() const;
static StaticAttributes concatAttributes(const Mesh::StaticAttributes& current,
                                         const Mesh::StaticAttributes& other);

static DynamicAttributes concatAttributes(const Mesh::DynamicAttributes& current,
                                          const Mesh::DynamicAttributes& other);

private:
size_t computeNumberOfVertices() const;
template<typename T> static void concatVector(std::vector <T>& current, const std::vector <T>& data);

template<typename T> void duplicateDynamicAttribute(std::vector <std::vector <T> >& attributes,
                                                    const std::vector <T>& values) const;

template<typename RawType, typename OpenGLType> static void convertAttributesToOpenGLFormat(
    const std::vector <RawType>& rawValues, std::vector <OpenGLType>& openGLValues);

template<typename RawType, typename OpenGLType> static void convertUniformsToOpenGLFormat(
    const std::map <std::string, RawType>& rawValues, std::map <std::string, OpenGLType>& openGLValues);

const State& _state;
const vecCstGeometricShape_sptr _shapes;
const size_t _numberOfVertices;
};

#endif // MESH_H
