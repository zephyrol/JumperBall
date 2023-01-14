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

    template<typename T> using GpuUniformValues = std::vector<T>;
    using GpuUniformNames = std::vector<std::string>;

    template<typename T>
    static std::vector<T> extractUniformVariablesValues(
        const GpuUniformValues<T> &uniformVariables
    );

    struct UniformsValues {
        GpuUniformValues<GLint> uniformInts = {};
        GpuUniformValues<GLfloat> uniformFloats = {};
        GpuUniformValues<glm::vec2> uniformVec2s = {};
        GpuUniformValues<glm::vec3> uniformVec3s = {};
        GpuUniformValues<glm::vec4> uniformVec4s = {};
        GpuUniformValues<glm::mat4> uniformMat4s = {};
        GpuUniformValues<GLuint> uniformTextures = {};
    };

    struct UniformsNames {
        GpuUniformNames uniformsInts = {};
        GpuUniformNames uniformFloats = {};
        GpuUniformNames uniformVec2s = {};
        GpuUniformNames uniformVec3s = {};
        GpuUniformNames uniformVec4s = {};
        GpuUniformNames uniformMat4s = {};
        GpuUniformNames uniformTextures = {};
    };

    Displayable::GlobalState getGlobalState() const;

    UniformsValues genUniformsValues() const;

    UniformsNames genUniformsNames() const;

    MeshGeometry genMeshGeometry() const;

    bool updatingIsUseless() const;

private:

    template<typename RawType, typename OpenGLType>
    static void convertUniformsToOpenGLFormat(
        const Displayable::DynamicValues<RawType> &rawValues,
        Mesh::GpuUniformValues<OpenGLType> &openGLValue
    );

    const CstDisplayable_sptr _displayable;
    const vecCstGeometricShape_sptr _shapes;
    const bool _updatingIsUseless;
};

template<typename T>
std::vector<T> Mesh::extractUniformVariablesValues(
    const GpuUniformValues<T> &uniformVariables
) {
    std::vector<T> uniformVariablesValues;
    for (const auto &uniformVariable: uniformVariables) {
        const T &uniformVariableValue = uniformVariable.second;
        uniformVariablesValues.push_back(uniformVariableValue);
    }

    return uniformVariablesValues;
}

template<typename RawType, typename OpenGLType>
void Mesh::convertUniformsToOpenGLFormat(
    const Displayable::DynamicValues<RawType> &rawValues,
    Mesh::GpuUniformValues<OpenGLType> &openGLValues) {
    for(const auto & rawValue: rawValues) {
        const OpenGLType openGLValue = Utility::convertToOpenGLFormat(rawValue);
        openGLValues.push_back(openGLValue);
    }
}


#endif // MESH_H
