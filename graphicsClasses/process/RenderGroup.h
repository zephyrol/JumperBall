/*
 * File: RenderGroup.h
 * Author: Morgenthaler S
 *
 * Created on 20 mars 2021, 16:30
 */

#ifndef RENDERGROUP_H
#define RENDERGROUP_H

#include "componentsGeneration/MeshGenerator.h"
#include "process/scene/GpuVertexAttributes.h"
#include "RenderGroupUniforms.h"

class RenderGroup;
using RenderGroup_sptr = std::shared_ptr<RenderGroup>;
using CstRenderGroup_sptr = std::shared_ptr<const RenderGroup>;
using vecRenderGroup_sptr = std::vector<RenderGroup_sptr>;
using vecCstRenderGroup_sptr = std::vector<CstRenderGroup_sptr>;
class RenderGroup {

public:

    RenderGroup(
        MeshDynamicGroup_uptr meshDynamicGroup,
        GLuint vertexArrayObject,
        vecGpuVertexAttributes_sptr gpuVertexAttributes,
        GLuint elementBufferObject,
        GLsizei numberOfIndices
    );

    RenderGroup(const RenderGroup &renderGroup) = delete;

    RenderGroup &operator=(const RenderGroup &renderGroup) = delete;

    void freeGPUMemory();

    void render() const;

    short numberOfDynamicsIds() const;

    RenderGroupUniforms genUniforms(const CstShaderProgram_sptr& shaderProgram) const;

    static RenderGroup_sptr createInstance(MeshDynamicGroup_uptr meshDynamicGroup);

private:

    const MeshDynamicGroup_uptr _meshDynamicGroup;
    const GLuint _vertexArrayObject;
    const vecGpuVertexAttributes_sptr _gpuVertexAttributes;
    const GLuint _elementBufferObject;
    const GLsizei _numberOfIndices;
};


#endif // RENDERGROUP_H
