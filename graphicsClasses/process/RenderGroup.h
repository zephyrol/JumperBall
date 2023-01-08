/*
 * File: RenderGroup.h
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */

#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include "process/mesh/Mesh.h"

class RenderGroup {

public:

    explicit RenderGroup(
        vecMesh_sptr meshes,
        GLuint vertexArrayObject,
        std::vector<GLuint> vertexBufferObjects,
        GLuint elementBufferObject,
        GLsizei numberOfIndices
    );

    RenderGroup(const RenderGroup &renderGroup) = delete;

    RenderGroup &operator=(const RenderGroup &renderGroup) = delete;

    RenderGroup(RenderGroup &&renderGroup) = default;

    void freeGPUMemory();

    void render() const;

    CstMesh_sptr getHeadMesh() const;

    static RenderGroup createInstance(vecMesh_sptr meshes);

private:

    template<typename T>
    static void fillBufferObjectData(
        GLuint bo,
        const std::vector<T> &bufferObjectData,
        GLenum target
    );

    template<typename T>
    void activateVertexAttribute(
        const std::vector<T> &attributeData,
        GLuint attributeNumber
    ) const;

    template<typename T>
    bool updateBufferObjectData(
        GLuint bo,
        const std::vector<T> &bufferObjectData,
        GLenum target
    ) const;

    const vecMesh_sptr _meshes;
    const GLuint _vertexArrayObject;
    const std::vector<GLuint> _vertexBufferObjects;
    const GLuint _elementBufferObject;
    const GLsizei _numberOfIndices;
};


#endif // RENDERGROUP_H
