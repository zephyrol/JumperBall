/*
 * File: RenderGroup.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */
#include "process/RenderGroup.h"

#include "GpuVertexArray.h"
#include "RenderGroupUniforms.h"

RenderGroup::RenderGroup(MeshDynamicGroup_uptr meshDynamicGroup,
                         vecCstGpuBuffer_uptr gpuGeometryBuffers,
                         CstGpuVertexArray_uptr gpuVertexArray,
                         GLsizei numberOfIndices)
    : _meshDynamicGroup(std::move(meshDynamicGroup)),
      _gpuGeometryBuffers(std::move(gpuGeometryBuffers)),
      _gpuVertexArray(std::move(gpuVertexArray)),
      _numberOfIndices(numberOfIndices) {}

void RenderGroup::bind() const {
    _gpuVertexArray->bind();
}

void RenderGroup::render() const {
    glDrawElements(GL_TRIANGLES, _numberOfIndices, GL_UNSIGNED_SHORT, nullptr);
}

RenderGroup_sptr RenderGroup::createInstance(MeshDynamicGroup_uptr meshDynamicGroup) {
    // 1. VAO
    auto gpuVertexArray = CstGpuVertexArray_uptr(new GpuVertexArray());
    gpuVertexArray->bind();

    const auto& meshes = meshDynamicGroup->meshes();

    // 2. Merge all geometries
    auto groupGeometry = std::accumulate(meshes.begin() + 1, meshes.end(), meshes.front()->genMeshGeometry(),
                                         [](MeshGeometry& current, const CstMesh_sptr& other) {
                                             current.merge(other->genMeshGeometry());
                                             return std::move(current);
                                         });

    // 3. Create EBO
    const auto& indices = groupGeometry.indices();
    vecCstGpuBuffer_uptr gpuGeometryBuffers;
    gpuGeometryBuffers.emplace_back(createElementBuffer(indices));

    // 4. Create gpu vertex attributes
    auto vertexAttributes = groupGeometry.extractVertexAttributes();

    for (size_t i = 0; i < vertexAttributes.size(); ++i) {
        auto& vertexAttribute = vertexAttributes[i];
        gpuGeometryBuffers.emplace_back(
            createGeometryBuffer(std::move(vertexAttribute), static_cast<GLuint>(i)));
    }

    return std::make_shared<RenderGroup>(std::move(meshDynamicGroup), std::move(gpuGeometryBuffers),
                                         std::move(gpuVertexArray), static_cast<GLsizei>(indices.size()));
}

CstGpuBuffer_uptr RenderGroup::createElementBuffer(const GeometricShape::IndicesBuffer& indices) {
    // 1. Create buffer object.
    auto bo = CstGpuBuffer_uptr(new GpuBuffer());

    // 2. Bind buffer object.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo->getId());

    GeometricShape::IndicesBuffer test{0};
    // 3. Create data on gpu
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 //static_cast<GLsizeiptr>(indices.size() * sizeof(decltype(indices.front()))), indices.data(),
                 static_cast<GLsizeiptr>(test.size() * sizeof(decltype(test.front()))), test.data(),
                 GL_STATIC_DRAW);

    return bo;
}

CstGpuBuffer_uptr RenderGroup::createGeometryBuffer(const CstVertexAttributeBase_uptr& vertexAttribute,
                                                    GLuint index) {
    // 1. Create buffer object.
    auto bo = CstGpuBuffer_uptr(new GpuBuffer());

    // 2. Bind buffer object.
    glBindBuffer(GL_ARRAY_BUFFER, bo->getId());

    // 3. Create data on gpu
    vertexAttribute->createDataOnGpu();

    // 4. Link and activate this vbo in the vao at the index location.
    vertexAttribute->getVertexAttribPointerFunc()(index);
    glEnableVertexAttribArray(index);

    return bo;
}

RenderGroupUniforms RenderGroup::genUniforms(const ShaderProgram_uptr& shaderProgram) const {
    return RenderGroupUniforms::createInstance(*_meshDynamicGroup, shaderProgram);
}

short RenderGroup::numberOfDynamicsIds() const {
    return _meshDynamicGroup->numberOfDynamicsIds();
}
