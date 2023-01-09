/*
 * File: RenderGroup.h
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */

#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include "process/mesh/Mesh.h"
#include "process/scene/GpuVertexAttributes.h"

class RenderGroup {

public:

    explicit RenderGroup(
        vecMesh_sptr meshes,
        GLuint vertexArrayObject,
        vecGpuVertexAttributes_sptr gpuVertexAttributes,
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

    const vecMesh_sptr _meshes;
    const GLuint _vertexArrayObject;
    const vecGpuVertexAttributes_sptr _gpuVertexAttributes;
    const GLuint _elementBufferObject;
    const GLsizei _numberOfIndices;
};


#endif // RENDERGROUP_H
