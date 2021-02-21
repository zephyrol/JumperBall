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
struct ShapeVertexAttributes {
    std::vector <glm::vec3> positions;
    std::vector <glm::vec3> normals;
    std::vector <glm::vec3> colors;
    std::vector <glm::vec2> uvCoords;
    std::vector <GLushort> indices;
};

struct StateVertexAttributes {
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
// ShapeVertexAttributes genShapeVertexAttributes() const;
// StateVertexAttributes genStateVertexAttributes() const;
Uniforms genUniformsValues() const;
static ShapeVertexAttributes concatAttributes(const Mesh::ShapeVertexAttributes& current,
                                              const Mesh::ShapeVertexAttributes& other);

static StateVertexAttributes concatAttributes(const Mesh::StateVertexAttributes& current,
                                              const Mesh::StateVertexAttributes& other);

private:

size_t computeNumberOfVertices() const;

template<typename T> static void concatVector(std::vector <T>& current, const std::vector <T>& other);
static void concatIndices(
    std::vector <GLushort>& currentIndices,
    const std::vector <GLushort>& otherIndices,
    size_t offset
    );

template<typename T> static void concatIndependantShapeVertexAttribute(std::vector <T>& staticAttributeData,
                                                                       const std::shared_ptr <const std::vector <T> >& shapeData);
static void concatIndicesShapeVertexAttribute(std::vector <GLushort>& staticAttributeData,
                                              const std::shared_ptr <const std::vector <GLushort> >& shapeData,
                                              size_t offset);

template<typename T> void duplicateStateVertexAttribute(std::vector <std::vector <T> >& attributes,
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
