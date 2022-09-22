/*
 * File: Mesh.h
 * Author: Morgenthaler S
 *
 * Created on 31 janvier 2021, 18:45
 */

#ifndef MESH_H
#define MESH_H

#include "ShaderProgram.h"
#include "geometry/GeometricShape.h"
#include "scene/SceneElement.h"

class Mesh;

using Mesh_sptr = std::shared_ptr<Mesh>;
using CstMesh_sptr = std::shared_ptr<const Mesh>;
using vecCstMesh_sptr = std::vector<CstMesh_sptr>;
using vecMesh_sptr = std::vector<Mesh_sptr>;

class Mesh {

public:
    Mesh(const CstSceneElement_sptr &sceneElement, vecCstGeometricShape_sptr &&shapes);

    struct StateVertexAttributes {
        std::vector<std::vector<GLfloat> > staticFloats = {};
        std::vector<std::vector<glm::vec3> > staticVec3s = {};
        std::vector<std::vector<glm::vec2> > staticVec2s = {};
    };

    struct MeshVerticesInfo {
        GeometricShape::ShapeVerticesInfo shapeVerticesInfo;
        Mesh::StateVertexAttributes stateVertexAttributes;
    };

    template<typename T> using UniformVariables = std::unordered_map<std::string, T>;
    template<typename T> using UniformVariables_uptr = std::unique_ptr<UniformVariables<T> >;

    template<typename T>
    static std::vector<T> extractUniformVariablesValues(
        const UniformVariables<T> &uniformVariables
    );

    struct Uniforms {
        UniformVariables<GLfloat> uniformFloats = {};
        UniformVariables<glm::vec2> uniformVec2s = {};
        UniformVariables<glm::vec3> uniformVec3s = {};
        UniformVariables<glm::vec4> uniformVec4s = {};
        UniformVariables<glm::mat4> uniformMat4s = {};
        UniformVariables<GLuint> uniformTextures = {};
    };

    SceneElement::GlobalState getGlobalState() const;

    size_t numberOfVertices() const;

    Uniforms genUniformsValues() const;

    MeshVerticesInfo genMeshVerticesInfo() const;


    bool updatingIsUseless() const;

    static void concatMeshVerticesInfo(MeshVerticesInfo &current, const MeshVerticesInfo &other);

private:

    size_t computeNumberOfVertices() const;

    template<typename T>
    void duplicateStateVertexAttribute(
        std::vector<std::vector<T> > &attributes,
        const std::vector<T> &values
    ) const;

    template<typename RawType, typename OpenGLType>
    static void convertAttributesToOpenGLFormat(
        const SceneElement::StaticValues<RawType> &rawValues,
        std::vector<OpenGLType> &openGLValues
    );

    template<typename RawType, typename OpenGLType>
    static void convertUniformsToOpenGLFormat(
        const SceneElement::DynamicValues<RawType> &rawValues,
        Mesh::UniformVariables<OpenGLType> &openGLValue
    );

    template<typename T>
    static void concatStateVertexAttribute(
        std::vector<std::vector<T> > &current,
        const std::vector<std::vector<T> > &other
    );

    static void concatStateVertexAttributes(
        StateVertexAttributes &current,
        const StateVertexAttributes &other
    );

    const CstSceneElement_sptr _sceneElement;
    const vecCstGeometricShape_sptr _shapes;
    const size_t _numberOfVertices;
    const bool _updatingIsUseless;
};

template<typename T>
std::vector<T> Mesh::extractUniformVariablesValues(
    const UniformVariables<T> &uniformVariables
) {
    std::vector<T> uniformVariablesValues;
    for (const auto &uniformVariable: uniformVariables) {
        const T &uniformVariableValue = uniformVariable.second;
        uniformVariablesValues.push_back(uniformVariableValue);
    }

    return uniformVariablesValues;
}


#endif // MESH_H
