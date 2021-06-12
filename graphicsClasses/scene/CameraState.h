/*
 * File: CameraState.h
 * Author: Morgenthaler S
 *
 * Created on 14 juin 2021, 10:05
 */

#ifndef CAMERASTATE_H
#define CAMERASTATE_H
#include "Camera.h"

class CameraState: public ObjectState {

public:

CameraState(const Camera& camera);

virtual ObjectState::GlobalState update() override;
const glm::vec3& pos() const noexcept;
const glm::mat4& viewProjection() const noexcept;

private:
const Camera& _camera;
glm::vec3 _pos;
glm::mat4 _viewProjection;

};
#endif /* CAMERA_H */
