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
    std::vector<GLuint> vertexBufferObjects,
    GLuint elementBufferObject,
    GLsizei numberOfIndices
) :
    _meshes(std::move(meshes)),
    _vertexArrayObject(vertexArrayObject),
    _vertexBufferObjects(std::move(vertexBufferObjects)),
    _elementBufferObject(elementBufferObject),
    _numberOfIndices(numberOfIndices) {
}

CstMesh_sptr RenderGroup::getHeadMesh() const {
    return _meshes.at(0);
}

template<typename T>
void RenderGroup::activateVertexAttribute(
    const std::vector<T> &,
    GLuint attributeNumber
) const {
    const size_t numberOfGLFloats = sizeof(T) / sizeof(GLfloat);
    glEnableVertexAttribArray(attributeNumber);
    glVertexAttribPointer(attributeNumber, numberOfGLFloats, GL_FLOAT, GL_FALSE, 0, nullptr);
}

template<>
void RenderGroup::activateVertexAttribute(
    const std::vector<int> &,
    GLuint attributeNumber
) const {
    constexpr size_t numberOfInt = 1;
    glEnableVertexAttribArray(attributeNumber);
    glVertexAttribPointer(attributeNumber, numberOfInt, GL_INT, GL_FALSE, 0, nullptr);
}

template<typename T>
bool RenderGroup::updateBufferObjectData(
    GLuint bo,
    const std::vector<T> &bufferObjectData,
    GLenum target
) const {
    if (!bufferObjectData.empty()) {
        fillBufferObjectData(bo, bufferObjectData, target);
        return true;
    }
    return false;
}

template<typename T>
void RenderGroup::fillBufferObjectData(
    GLuint bo,
    const std::vector<T> &bufferObjectData,
    GLenum target
) {
    glBindBuffer(target, bo);
    glBufferData(target, bufferObjectData.size() * sizeof(T), bufferObjectData.data(), GL_STATIC_DRAW);
}

void RenderGroup::render() const {
    glBindVertexArray(_vertexArrayObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);
    glDrawElements(GL_TRIANGLES, _numberOfIndices, GL_UNSIGNED_SHORT, nullptr);
}

void RenderGroup::freeGPUMemory() {
    glDeleteBuffers(1, &_elementBufferObject);
    for (const auto &vertexBufferObject: _vertexBufferObjects) {
        glDeleteBuffers(1, &vertexBufferObject);
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

    // 2. VBOs
    auto groupGeometry = std::accumulate(
        meshes.begin() + 1,
        meshes.end(),
        meshes.front()->genMeshGeometry(),
        [](MeshGeometry &current, const CstMesh_sptr &other) {
            current.merge(other->genMeshGeometry());
            return std::move(current);
        });

    const auto vertexAttributes = groupGeometry.extractVertexAttributes();
    std::vector<GLuint> vertexBufferObjects(vertexAttributes.size());

    const auto genAndBindBufferObject = []() {
        GLuint bo;
        glGenBuffers(1, &bo);
        glBindBuffer(GL_ARRAY_BUFFER, bo);
        return bo;
    };

    std::transform(
        vertexAttributes.cbegin(),
        vertexAttributes.cend(),
        vertexBufferObjects.begin(),
        [&genAndBindBufferObject](const VertexAttributeBase_uptr &vertexAttribute) {
            const auto vbo = genAndBindBufferObject();
            vertexAttribute->createDataOnGpu();
            return vbo;
        }
    );

    // EBO
    const auto indices = groupGeometry.indices();
    const auto elementBufferObject = genAndBindBufferObject();
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(decltype(indices.front()))),
        indices.data(),
        GL_STATIC_DRAW
    );

    return RenderGroup(
        std::move(meshes),
        vertexArrayObject,
        vertexBufferObjects,
        elementBufferObject,
        static_cast<GLsizei>(indices.size())
    );
}

