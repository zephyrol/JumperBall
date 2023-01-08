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
#include "scene/Displayable.h"
#include "MeshGeometry.h"

class Mesh;

using Mesh_sptr = std::shared_ptr<Mesh>;
using CstMesh_sptr = std::shared_ptr<const Mesh>;
using vecCstMesh_sptr = std::vector<CstMesh_sptr>;
using vecMesh_sptr = std::vector<Mesh_sptr>;

class Mesh {

public:
    Mesh(const CstDisplayable_sptr &displayable, vecCstGeometricShape_sptr &&shapes);

    Mesh(const Mesh &mesh) = delete;

    Mesh &operator=(const Mesh &mesh) = delete;

    template<typename T> using UniformVariables = std::unordered_map<std::string, T>;
    template<typename T> using UniformVariables_uptr = std::unique_ptr<UniformVariables<T> >;

    template<typename T>
    static std::vector<T> extractUniformVariablesValues(
        const UniformVariables<T> &uniformVariables
    );

    struct Uniforms {
        UniformVariables<GLint> uniformInts = {};
        UniformVariables<GLfloat> uniformFloats = {};
        UniformVariables<glm::vec2> uniformVec2s = {};
        UniformVariables<glm::vec3> uniformVec3s = {};
        UniformVariables<glm::vec4> uniformVec4s = {};
        UniformVariables<glm::mat4> uniformMat4s = {};
        UniformVariables<GLuint> uniformTextures = {};
    };

    Displayable::GlobalState getGlobalState() const;

    Uniforms genUniformsValues() const;

    MeshGeometry genMeshGeometry() const;

    bool updatingIsUseless() const;

private:

    template<typename RawType, typename OpenGLType>
    static void convertUniformsToOpenGLFormat(
        const Displayable::DynamicValues<RawType> &rawValues,
        Mesh::UniformVariables<OpenGLType> &openGLValue
    );

    const CstDisplayable_sptr _displayable;
    const vecCstGeometricShape_sptr _shapes;
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
