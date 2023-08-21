/*
 * File: RenderGroup.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */
#include "process/RenderGroup.h"
#include "RenderGroupUniforms.h"

RenderGroup::RenderGroup(
    MeshDynamicGroup_uptr meshDynamicGroup,
    GLuint vertexArrayObject,
    vecGpuVertexBuffer_sptr gpuVertexBuffers,
    GLuint elementBufferObject,
    GLsizei numberOfIndices
) :
    _meshDynamicGroup(std::move(meshDynamicGroup)),
    _vertexArrayObject(vertexArrayObject),
    _gpuVertexBuffers(std::move(gpuVertexBuffers)),
    _elementBufferObject(elementBufferObject),
    _numberOfIndices(numberOfIndices) {
}

void RenderGroup::render() const {
    glBindVertexArray(_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, _numberOfIndices, GL_UNSIGNED_SHORT, nullptr);
}

void RenderGroup::freeGPUMemory() {
    glDeleteBuffers(1, &_elementBufferObject);
    for (const auto &gpuVertexAttribute: _gpuVertexBuffers) {
        gpuVertexAttribute->freeGPUMemory();
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

    // 3. Create gpu vertex attributes
    const auto genBufferObject = []() {
        GLuint bo;
        glGenBuffers(1, &bo);
        return bo;
    };

    auto vertexAttributes = groupGeometry.extractVertexAttributes();
    vecGpuVertexBuffer_sptr gpuVertexAttributes{};

    for (size_t i = 0; i < vertexAttributes.size(); ++i) {
        auto &vertexAttribute = vertexAttributes[i];
        gpuVertexAttributes.push_back( GpuVertexBuffer::createInstance(
            std::move(vertexAttribute),
            static_cast<GLuint>(i)
        ) );
    }

    // 4. Create EBO
    const auto& indices =  groupGeometry.indices();

    const auto elementBufferObject = genBufferObject();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(decltype(indices.front()))),
        indices.data(),
        GL_STATIC_DRAW
    );

    return std::make_shared<RenderGroup>(
        std::move(meshDynamicGroup),
        vertexArrayObject,
        std::move(gpuVertexAttributes),
        elementBufferObject,
        static_cast<GLsizei>(indices.size())
    );
}

RenderGroupUniforms RenderGroup::genUniforms(const CstShaderProgram_sptr &shaderProgram) const {
    return RenderGroupUniforms::createInstance(*_meshDynamicGroup, shaderProgram);
}

short RenderGroup::numberOfDynamicsIds() const {
    return _meshDynamicGroup->numberOfDynamicsIds();
}
