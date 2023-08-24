//
// Created by S.Morgenthaler on 02/07/2023.
//

#ifndef JUMPERBALLAPPLICATION_MESHDYNAMICGROUP_H
#define JUMPERBALLAPPLICATION_MESHDYNAMICGROUP_H

#include "Mesh.h"

class MeshDynamicGroup;

using MeshDynamicGroup_uptr = std::unique_ptr<MeshDynamicGroup>;

class MeshDynamicGroup {

public:
    explicit MeshDynamicGroup(
        std::vector<vecCstMesh_sptr> &&dynamicMeshes
    );

    const std::vector<vecCstMesh_sptr> &dynamicMeshes() const;

    const vecCstMesh_sptr &meshes() const;

    short numberOfDynamicsIds() const;

    MeshDynamicGroup(const MeshDynamicGroup &meshDynamicGroup) = delete;

    const MeshDynamicGroup &operator=(const MeshDynamicGroup &meshDynamicGroup) = delete;

    MeshDynamicGroup(MeshDynamicGroup &&meshDynamicGroup) = default;


private:

    /**
     * Meshes where the uniforms (dynamics values) may change. It's important to update those uniforms
     * each frame. Each element of the vector contain the meshes with the same dynamics id.
     */
    const std::vector<vecCstMesh_sptr> _dynamicMeshes;

    /**
     * All the meshes.
     */
    const vecCstMesh_sptr _meshes;

    const short _numberOfDynamicsIds;
};


#endif //JUMPERBALLAPPLICATION_MESHDYNAMICGROUP_H
