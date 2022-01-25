/*
 * File: RenderGroup.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */
#include "RenderGroup.h"

RenderGroup::RenderGroup(const vecMesh_sptr& meshes):
    _meshes(meshes),
    _meshesVerticesInfo(createMeshesVerticesInfo()),
    _vertexArrayObject(genVertexArrayObject()),
    _bufferObjects(createBufferObjects())
{}

CstMesh_sptr RenderGroup::getHeadMesh() const {
    return _meshes.at(0);
}

/*vecMesh_sptr RenderGroup::update (const vecMesh_sptr& meshesToAdd) {
    vecMesh_sptr rejectedMeshes;
    bool needBuffersRebinding = false;

    for (auto it = _meshes.begin(); it != _meshes.end();) {
        const Mesh_sptr& mesh = *it;
        const SceneElement::GlobalState meshState = mesh->update();
        if (meshState != _globalState) {
            if (meshState != SceneElement::GlobalState::Dead) {
                rejectedMeshes.push_back(mesh);
            }
            _meshes.erase(it);
            needBuffersRebinding = true;
        } else {
            ++it;
        }
    }
    if (!meshesToAdd.empty()) {
        _meshes.insert(_meshes.end(), meshesToAdd.begin(), meshesToAdd.end());
        needBuffersRebinding = true;
    }

    if (needBuffersRebinding) {
        _meshesVerticesInfo = createMeshesVerticesInfo();
    }
    _needBuffersRebinding = needBuffersRebinding;
    return rejectedMeshes;
}*/

GLuint RenderGroup::genVertexArrayObject() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}

GLuint RenderGroup::genBufferObject() {
    GLuint bo;
    glGenBuffers(1, &bo);
    return bo;
}

Mesh::MeshVerticesInfo RenderGroup::createMeshesVerticesInfo() const {
    Mesh::MeshVerticesInfo meshesVerticesInfo;
    for (const auto& mesh : _meshes) {
        Mesh::concatMeshVerticesInfo(meshesVerticesInfo, mesh->genMeshVerticesInfo());
    }
    return meshesVerticesInfo;
}

const RenderGroup::BufferObjects& RenderGroup::bufferObjects() const {
    return _bufferObjects;
}

template<typename T> std::vector <GLuint> RenderGroup::createStateVertexAttributesBufferObject (
    const std::vector <std::vector <T> >& attributes) const {
    std::vector <GLuint> vertexBufferObjects;
    for (const auto& stateVertexAttribute : attributes) {
        const auto vertexBuffer = initializeVBO(stateVertexAttribute);
        if (vertexBuffer) {
            vertexBufferObjects.push_back(*vertexBuffer);
        }
    }
    return vertexBufferObjects;
}

template<typename T> void RenderGroup::fillVBOsList (
    std::vector <GLuint>& vboList,
    const std::vector <T>& attributeData,
    size_t attributesOffset
    ) const {
    const std::shared_ptr <GLuint> vertexBufferObject = initializeVBO(attributeData);
    if (vertexBufferObject) {
        const size_t attributeNumber = vboList.size() + attributesOffset;
        activateVertexAttribute(attributeData, attributeNumber);
        vboList.push_back(*vertexBufferObject);
    }
}

template<typename T> void RenderGroup::fillStateVertexAttributesVBOsList (
    std::vector <GLuint>& vboList,
    const std::vector <std::vector <T> >& stateVertexAttributes,
    size_t attributesOffset) const {
    for (const std::vector <T>& stateVertexAttribute : stateVertexAttributes) {
        fillVBOsList(vboList, stateVertexAttribute, attributesOffset);
    }
}

RenderGroup::BufferObjects RenderGroup::createBufferObjects() const {

    glBindVertexArray(_vertexArrayObject);
    RenderGroup::BufferObjects bufferObjects;

    std::vector <GLuint>& shapeVBOs = bufferObjects.shapeVertexBufferObjects;
    const auto& shapeVerticesInfo = _meshesVerticesInfo.shapeVerticesInfo;
    const auto& shapeVertexAttributes = shapeVerticesInfo.shapeVertexAttributes;
    fillVBOsList(shapeVBOs, shapeVertexAttributes.positions);
    fillVBOsList(shapeVBOs, shapeVertexAttributes.colors);
    fillVBOsList(shapeVBOs, shapeVertexAttributes.normals);
    fillVBOsList(shapeVBOs, shapeVertexAttributes.uvCoords);

    const size_t attributesOffset = shapeVBOs.size();
    std::vector <GLuint>& stateVBOs = bufferObjects.stateVertexBufferObjects;
    const auto& stateVertexAttributes = _meshesVerticesInfo.stateVertexAttributes;

    fillStateVertexAttributesVBOsList(stateVBOs, stateVertexAttributes.staticFloats, attributesOffset);
    fillStateVertexAttributesVBOsList(stateVBOs, stateVertexAttributes.staticVec2s, attributesOffset);
    fillStateVertexAttributesVBOsList(stateVBOs, stateVertexAttributes.staticVec3s, attributesOffset);

    const auto ebo = initializeEBO(shapeVerticesInfo.indices);
    bufferObjects.elementBufferObject = ebo
                                        ? *ebo
                                        : 0;

    const auto& positions = shapeVertexAttributes.positions;
    const auto& colors = shapeVertexAttributes.colors;
    const auto& normals = shapeVertexAttributes.normals;
    const auto& uvCoords = shapeVertexAttributes.uvCoords;
    size_t nbOfShapes = 0;
    nbOfShapes += updateShapeVBOsData(shapeVBOs, positions, nbOfShapes);
    nbOfShapes += updateShapeVBOsData(shapeVBOs, colors, nbOfShapes);
    nbOfShapes += updateShapeVBOsData(shapeVBOs, normals, nbOfShapes);
    nbOfShapes += updateShapeVBOsData(shapeVBOs, uvCoords, nbOfShapes);

    size_t nbOfStates = 0;
    nbOfStates += updateStateVBOsData(stateVBOs, stateVertexAttributes.staticFloats, nbOfShapes, nbOfStates);
    nbOfStates += updateStateVBOsData(stateVBOs, stateVertexAttributes.staticVec2s, nbOfShapes, nbOfStates);
    nbOfStates += updateStateVBOsData(stateVBOs, stateVertexAttributes.staticVec3s, nbOfShapes, nbOfStates);

    const auto& indices = shapeVerticesInfo.indices;
    updateBufferObjectData(*ebo, indices, GL_ELEMENT_ARRAY_BUFFER);

    return bufferObjects;
}

template<typename T> void RenderGroup::activateVertexAttribute (const std::vector <T>&,
                                                                GLuint attributeNumber) const {
    const size_t numberOfGLFloats = sizeof(T) / sizeof(GLfloat);
    glEnableVertexAttribArray(attributeNumber);
    glVertexAttribPointer(attributeNumber, numberOfGLFloats, GL_FLOAT, GL_FALSE, 0, nullptr);
}

template<typename T> size_t RenderGroup::updateBufferObjectData (
    GLuint bo,
    const std::vector <T>& bufferObjectData,
    GLenum target
    ) const {
    if (!bufferObjectData.empty()) {
        fillBufferObjectData(bo, bufferObjectData, target);
        return 1;
    }
    return 0;
}

template<typename T> size_t RenderGroup::updateStateVBOsData (
    const std::vector <GLuint>& vbosList,
    const std::vector <std::vector <T> >& bufferObjectsData,
    size_t nbOfShapeVbos,
    size_t attributesOffset
    ) const {
    size_t vbosDataUpdated = 0;
    for (size_t i = 0; i < bufferObjectsData.size(); ++i) {
        const auto& bufferObjectData = bufferObjectsData.at(i);

        const size_t stateVboNumber = i + attributesOffset;
        const auto& vbo = vbosList.at(stateVboNumber);

        if (updateBufferObjectData(vbo, bufferObjectData, GL_ARRAY_BUFFER)) {
            const GLuint vertexAttributeNumber = nbOfShapeVbos + attributesOffset + vbosDataUpdated;
            activateVertexAttribute(bufferObjectData, vertexAttributeNumber);
            ++vbosDataUpdated;
        }
    }
    return vbosDataUpdated;
}

template<typename T> size_t RenderGroup::updateShapeVBOsData (const std::vector <GLuint>& vbosList,
                                                              const std::vector <T>& bufferObjectsData,
                                                              size_t attributesOffset) const {
    if (attributesOffset < vbosList.size()) {
        const GLuint vbo = vbosList.at(attributesOffset);
        if (updateBufferObjectData(vbo, bufferObjectsData, GL_ARRAY_BUFFER)) {
            activateVertexAttribute(bufferObjectsData, attributesOffset);
            return 1;
        }
    }
    return 0;
}


template<typename T> void RenderGroup::fillBufferObjectData (GLuint bo,
                                                             const std::vector <T>& bufferObjectData,
                                                             GLenum target) const {
    glBindBuffer(target, bo);
    glBufferData(target, bufferObjectData.size() * sizeof(T), bufferObjectData.data(), GL_STATIC_DRAW);
}

template<typename T> std::shared_ptr <GLuint> RenderGroup::initializeBO (
    const std::vector <T>& bufferObjectData,
    GLenum target
    ) const {
    std::shared_ptr <GLuint> bo = nullptr;
    if (!bufferObjectData.empty()) {
        bo = std::make_shared <GLuint>(genBufferObject());
        fillBufferObjectData(*bo, bufferObjectData, target);
    }
    return bo;
}

template<typename T> std::shared_ptr <GLuint> RenderGroup::initializeVBO (
    const std::vector <T>& attributeData
    ) const {
    return initializeBO(attributeData, GL_ARRAY_BUFFER);
}

std::shared_ptr <GLuint> RenderGroup::initializeEBO (const std::vector <GLushort>& indicesData) const {
    return initializeBO(indicesData, GL_ELEMENT_ARRAY_BUFFER);
}

void RenderGroup::render() const {
    glBindVertexArray(_vertexArrayObject);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferObjects.elementBufferObject);
    const auto numberOfIndices = static_cast<GLsizei>(_meshesVerticesInfo.shapeVerticesInfo.indices.size());
    glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_SHORT, nullptr);
}

void RenderGroup::freeGPUMemory() {
    glDeleteBuffers(1, &_bufferObjects.elementBufferObject);
    for(const auto& shapeVBO: _bufferObjects.shapeVertexBufferObjects){
        glDeleteBuffers(1, &shapeVBO);
    }
    for(const auto& stateVBO: _bufferObjects.stateVertexBufferObjects){
        glDeleteBuffers(1, &stateVBO);
    }
    glDeleteVertexArrays(1, &_vertexArrayObject);
}

vecMesh_sptr RenderGroup::createMeshesToUpdate() const {
    vecMesh_sptr meshesToUpdate {};
    for (const auto &mesh : _meshes) {
        if (!mesh->updatingIsUseless()) {
            meshesToUpdate.push_back(mesh);
        }
    }
    return meshesToUpdate;
}
