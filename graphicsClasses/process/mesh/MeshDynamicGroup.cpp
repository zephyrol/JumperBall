//
// Created by S.Morgenthaler on 02/07/2023.
//

#include "MeshDynamicGroup.h"

MeshDynamicGroup::MeshDynamicGroup(
    std::vector<vecCstMesh_sptr> &&dynamicMeshes
) :
    _dynamicMeshes(std::move(dynamicMeshes)),
    _meshes([this]() {
        vecCstMesh_sptr meshes{};
        for (const auto &meshList: _dynamicMeshes) {
            meshes.insert(meshes.end(), meshList.cbegin(), meshList.cend());
        }
        return meshes;
    }()),
    _numberOfDynamicsIds(static_cast<short>(_dynamicMeshes.size())) {
}

const vecCstMesh_sptr &MeshDynamicGroup::meshes() const {
    return _meshes;
}

const std::vector<vecCstMesh_sptr> &MeshDynamicGroup::dynamicMeshes() const {
    return _dynamicMeshes;
}

short MeshDynamicGroup::numberOfDynamicsIds() const {
    return _numberOfDynamicsIds;
}
