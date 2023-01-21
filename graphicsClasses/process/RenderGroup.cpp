/*
 * File: RenderGroup.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */
#include "process/RenderGroup.h"
#include <Utility.h>

RenderGroup::RenderGroup(
    vecMesh_sptr meshes,
    GLuint vertexArrayObject,
    vecGpuVertexAttributes_sptr gpuVertexAttributes,
    GLuint elementBufferObject,
    GLsizei numberOfIndices
) :
    _meshes(std::move(meshes)),
    _vertexArrayObject(vertexArrayObject),
    _gpuVertexAttributes(std::move(gpuVertexAttributes)),
    _elementBufferObject(elementBufferObject),
    _numberOfIndices(numberOfIndices) {
}

Mesh::UniformsValues RenderGroup::genUniformValues() const {
    return _meshes.front()->genUniformsValues();
}

void RenderGroup::render() const {
    glBindVertexArray(_vertexArrayObject);
    for (const auto &gpuVertexAttribute: _gpuVertexAttributes) {
        gpuVertexAttribute->bind();
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);
    glDrawElements(GL_TRIANGLES, _numberOfIndices, GL_UNSIGNED_SHORT, nullptr);

    for (const auto &gpuVertexAttribute: _gpuVertexAttributes) {
        gpuVertexAttribute->unbind();
    }
}

void RenderGroup::freeGPUMemory() {
    glDeleteBuffers(1, &_elementBufferObject);
    for (const auto &gpuVertexAttribute: _gpuVertexAttributes) {
        gpuVertexAttribute->freeGPUMemory();
    }
    glDeleteVertexArrays(1, &_vertexArrayObject);
}

RenderGroup RenderGroup::createInstance(vecMesh_sptr meshes) {

    // 1. VAO
    const auto genVertexArrayObject = []() {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        return vao;
    };

    const auto vertexArrayObject = genVertexArrayObject();
    glBindVertexArray(vertexArrayObject);

    // 2. Merge all geometries
    auto groupGeometry = std::accumulate(
        meshes.begin() + 1,
        meshes.end(),
        meshes.front()->genMeshGeometry(),
        [](MeshGeometry &current, const CstMesh_sptr &other) {
            current.merge(other->genMeshGeometry());
            return std::move(current);
        });

    // 3. Create gpu vertex attributes
    const auto genBufferObject = []() {
        GLuint bo;
        glGenBuffers(1, &bo);
        return bo;
    };

    auto vertexAttributes = groupGeometry.extractVertexAttributes();
    vecGpuVertexAttributes_sptr gpuVertexAttributes{};

    for(size_t i = 0; i < vertexAttributes.size(); ++i) {
        const auto vbo = genBufferObject();
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        auto& vertexAttribute = vertexAttributes[i];
        vertexAttribute->createDataOnGpu();
        gpuVertexAttributes.push_back(
            std::make_shared<GpuVertexAttributes>(vbo, std::move(vertexAttribute), i)
        );
    }

    // 4. Create EBO
    const auto indices = groupGeometry.indices();
    const auto elementBufferObject = genBufferObject();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(decltype(indices.front()))),
        indices.data(),
        GL_STATIC_DRAW
    );

    return RenderGroup(
        std::move(meshes),
        vertexArrayObject,
        std::move(gpuVertexAttributes),
        elementBufferObject,
        static_cast<GLsizei>(indices.size())
    );
}

