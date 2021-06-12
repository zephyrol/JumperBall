/*
 * File: CameraState.cpp
 * Author: Morgenthaler S
 *
 * Created on 12 juin 2021, 10:05
 */

#include "CameraState.h"

CameraState::CameraState(const Camera& camera):
_camera(camera),
_pos(camera.pos()),
_viewProjection(camera.viewProjection())
{
}

State::GlobalState CameraState::update() {
    _pos = _camera.pos();
    _viewProjection = _camera.viewProjection();
    return State::GlobalState::United;
}

const glm::vec3& CameraState::pos() const noexcept {
    return _pos;
}
const glm::mat4& CameraState::viewProjection() const noexcept {
    return _viewProjection;
}
