/*
 * File: RenderGroup.h
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */

#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include "process/Mesh.h"

class RenderGroup {

public:

    explicit RenderGroup(vecMesh_sptr meshes);

    RenderGroup(const RenderGroup &renderGroup) = delete;

    RenderGroup &operator=(const RenderGroup &renderGroup) = delete;

    RenderGroup(RenderGroup &&renderGroup) = default;

    void freeGPUMemory();

    void render() const;

    CstMesh_sptr getHeadMesh() const;


private:

    struct BufferObjects {
        std::vector<GLuint> shapeVertexBufferObjects;
        std::vector<GLuint> stateVertexBufferObjects;
        GLuint elementBufferObject;
    };

    template<typename T>
    void fillBufferObjectData(
        GLuint bo,
        const std::vector<T> &bufferObjectData,
        GLenum target
    ) const;

    template<typename T>
    std::shared_ptr<GLuint> initializeBO(
        const std::vector<T> &bufferObjectData,
        GLenum target
    ) const;

    template<typename T>
    std::shared_ptr<GLuint> initializeVBO(const std::vector<T> &attributeData) const;

    std::shared_ptr<GLuint> initializeEBO(const std::vector<GLushort> &indicesData) const;

    template<typename T>
    void fillVBOsList(
        std::vector<GLuint> &vboList,
        const std::vector<T> &attributeData,
        size_t attributesOffset = 0
    ) const;

    template<typename T>
    void fillStateVertexAttributesVBOsList(
        std::vector<GLuint> &vboList,
        const std::vector<std::vector<T> > &stateVertexAttributes,
        size_t attributesOffset
    ) const;

    BufferObjects createBufferObjects() const;

    template<typename T>
    void activateVertexAttribute(
        const std::vector<T> &attributeData,
        GLuint attributeNumber) const;

    template<typename T>
    size_t updateStateVBOsData(
        const std::vector<GLuint> &vbosList,
        const std::vector<std::vector<T> > &bufferObjectsData,
        size_t nbOfShapeVbos,
        size_t attributesOffset
    ) const;

    template<typename T>
    size_t updateShapeVBOsData(
        const std::vector<GLuint> &vbosList,
        const std::vector<T> &bufferObjectsData,
        size_t attributesOffset
    ) const;

    template<typename T>
    size_t updateBufferObjectData(
        GLuint bo,
        const std::vector<T> &bufferObjectData,
        GLenum target
    ) const;

    static GLuint genVertexArrayObject();

    static GLuint genBufferObject();

    Mesh::MeshVerticesInfo createMeshesVerticesInfo() const;

    const vecMesh_sptr _meshes;
    const Mesh::MeshVerticesInfo _meshesVerticesInfo;
    const GLuint _vertexArrayObject;
    const BufferObjects _bufferObjects;
};


#endif // RENDERGROUP_H
