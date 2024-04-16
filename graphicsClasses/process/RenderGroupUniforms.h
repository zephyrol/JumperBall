//
// Created by S.Morgenthaler on 02/07/2023.
//

#ifndef JUMPERBALLAPPLICATION_RENDERGROUPUNIFORMS_H
#define JUMPERBALLAPPLICATION_RENDERGROUPUNIFORMS_H


#include "process/mesh/MeshUniforms.h"
#include "process/mesh/MeshDynamicGroup.h"

class RenderGroupUniforms {

public:

    RenderGroupUniforms (
        vecMeshUniforms_sptr &&dynamicUniforms
    );

    static RenderGroupUniforms createInstance(
        const MeshDynamicGroup& meshDynamicGroup,
        const ShaderProgram_uptr &shaderProgram
    );

    void bind() const;

    void update();

private:

    const vecMeshUniforms_sptr _dynamicUniforms;
};


#endif //JUMPERBALLAPPLICATION_RENDERGROUPUNIFORMS_H
