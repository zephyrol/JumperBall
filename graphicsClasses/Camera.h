/*
 * File: Camera.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#ifndef CAMERA_H
#define CAMERA_H
#include <scene/BallState.h>
#include <scene/MapState.h>
#include <scene/StarState.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Utility.h"

class Camera {

public:

// --CONSTRUCTORS & DESTRUCTORS--//
Camera();


// ---------CONSTANTS------------//
static constexpr float zNear = 0.1f;
static constexpr float zFar = 100.f;


// -------CONST METHODS----------//
const glm::vec3& pos() const noexcept;
const glm::vec3& center() const noexcept;
const glm::vec3& up() const noexcept;
glm::mat4 viewProjection() const noexcept;


// ----------METHODS-------------//
void follow(const BallState& ballState) noexcept;
void follow(const MapState& mapState) noexcept;
bool transitionEffect(const BallState& ball,
                      const MapState& map) noexcept;

static glm::mat4 genVPMatrixFromStar(const StarState& starState);

private:

// --------ATTRIBUTES-----------//
const float _fovy;
const float _ratio;
glm::vec3 _pos;
glm::vec3 _center;
glm::vec3 _up;
bool _willComeBack;
bool _isComingBack;
float _cameraAboveWay;
JBTypes::timePointMs _timePointComeBack;

};
#endif /* CAMERA_H */
