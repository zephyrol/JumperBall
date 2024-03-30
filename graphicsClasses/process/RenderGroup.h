/*
 * File: RenderGroup.h
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */

#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include "GpuBuffer.h"
#include "GpuVertexArray.h"
#include "RenderGroupUniforms.h"

class RenderGroup;

using RenderGroup_uptr = std::unique_ptr<RenderGroup>;
using RenderGroup_sptr = std::shared_ptr<RenderGroup>;
using CstRenderGroup_sptr = std::shared_ptr<const RenderGroup>;
using vecRenderGroup_sptr = std::vector<RenderGroup_sptr>;
using vecCstRenderGroup_sptr = std::vector<CstRenderGroup_sptr>;

class RenderGroup {
public:
    RenderGroup(
        MeshDynamicGroup_uptr meshDynamicGroup,
        vecCstGpuBuffer_uptr gpuGeometryBuffers,
        CstGpuVertexArray_uptr gpuVertexArray,
        GLsizei numberOfIndices
    );

    RenderGroup(const RenderGroup &renderGroup) = delete;

    RenderGroup &operator=(const RenderGroup &renderGroup) = delete;

    void bind() const;

    void render() const;

    short numberOfDynamicsIds() const;

    RenderGroupUniforms genUniforms(const CstShaderProgram_sptr &shaderProgram) const;

    static RenderGroup_sptr createInstance(MeshDynamicGroup_uptr meshDynamicGroup);

private:
    static CstGpuBuffer_uptr createElementBuffer(const GeometricShape::IndicesBuffer &indices);
    static CstGpuBuffer_uptr createGeometryBuffer(
        const CstVertexAttributeBase_uptr &vertexAttribute,
        GLuint index
    );

    const MeshDynamicGroup_uptr _meshDynamicGroup;
    const vecCstGpuBuffer_uptr _gpuGeometryBuffers;
    const CstGpuVertexArray_uptr _gpuVertexArray;
    const GLsizei _numberOfIndices;
};


#endif // RENDERGROUP_H
