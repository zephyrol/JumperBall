/*
 * File: RenderGroup.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */
#include "process/RenderGroup.h"
#include "RenderGroupUniforms.h"
#include "process/mesh/gpuGeometryBuffers/GpuElementBuffer.h"

RenderGroup::RenderGroup(
    MeshDynamicGroup_uptr meshDynamicGroup,
    GLuint vertexArrayObject,
    vecGpuGeometryBuffer_sptr gpuGeometryBuffers,
    GLsizei numberOfIndices
) :
    _meshDynamicGroup(std::move(meshDynamicGroup)),
    _vertexArrayObject(vertexArrayObject),
    _gpuGeometryBuffers(std::move(gpuGeometryBuffers)),
    _numberOfIndices(numberOfIndices) {
}

void RenderGroup::render() const {
    glBindVertexArray(_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, _numberOfIndices, GL_UNSIGNED_SHORT, nullptr);
}

void RenderGroup::freeGPUMemory() {
    for (const auto &gpuGeometryBuffer: _gpuGeometryBuffers) {
       gpuGeometryBuffer->freeGPUMemory();
    }
    glDeleteVertexArrays(1, &_vertexArrayObject);
}

RenderGroup_sptr RenderGroup::createInstance(MeshDynamicGroup_uptr meshDynamicGroup) {

    // 1. VAO
    const auto genVertexArrayObject = []() {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        return vao;
    };

    const auto vertexArrayObject = genVertexArrayObject();
    glBindVertexArray(vertexArrayObject);

    const auto &meshes = meshDynamicGroup->meshes();

    // 2. Merge all geometries
    auto groupGeometry = std::accumulate(
        meshes.begin() + 1,
        meshes.end(),
        meshes.front()->genMeshGeometry(),
        [](MeshGeometry &current, const CstMesh_sptr &other) {
            current.merge(other->genMeshGeometry());
            return std::move(current);
        }
    );

    // 3. Create EBO
    const auto& indices =  groupGeometry.indices();
    vecGpuGeometryBuffer_sptr gpuGeometryBuffers{GpuElementBuffer::createInstance(indices)};

    // 4. Create gpu vertex attributes
    auto vertexAttributes = groupGeometry.extractVertexAttributes();

    for (size_t i = 0; i < vertexAttributes.size(); ++i) {
        auto &vertexAttribute = vertexAttributes[i];
        gpuGeometryBuffers.emplace_back(GpuVertexBuffer::createInstance(
            std::move(vertexAttribute),
            static_cast<GLuint>(i)
        ) );
    }

    return std::make_shared<RenderGroup>(
        std::move(meshDynamicGroup),
        vertexArrayObject,
        std::move(gpuGeometryBuffers),
        static_cast<GLsizei>(indices.size())
    );
}

RenderGroupUniforms RenderGroup::genUniforms(const CstShaderProgram_sptr &shaderProgram) const {
    return RenderGroupUniforms::createInstance(*_meshDynamicGroup, shaderProgram);
}

short RenderGroup::numberOfDynamicsIds() const {
    return _meshDynamicGroup->numberOfDynamicsIds();
}
