/*
 * File: Camera.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#ifndef CAMERA_H
#define CAMERA_H
#include <scene/BallState.h>
#include <scene/StarState.h>
#include <scene/Map.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Utility.h"

class Camera {

public:

enum class Movement { TurningAroundMap, FollowingBall, ApproachingBall };

Camera(const Map& map, const Ball& ball);

static constexpr float zNear = 0.1f;
static constexpr float zFar = 100.f;

const glm::vec3& pos() const noexcept;
glm::mat4 viewProjection() const noexcept;

void update() noexcept;
void turnAroundMap() noexcept;
void followBall() noexcept;
void approachBall() noexcept;
const Movement& getMovement() noexcept;

static glm::mat4 genVPMatrixFromStar(const StarState& starState);

private:

void turningAroundMapUpdate() noexcept;
void followingBallUpdate() noexcept;
bool approachingBallUpdate() noexcept;

const Map& _map;
const Ball& _ball;
const float _fovy;
const float _ratio;
Movement _movement;
glm::vec3 _pos;
glm::vec3 _center;
glm::vec3 _up;
bool _willComeBack;
bool _isComingBack;
float _cameraAboveWay;
float _timeSinceCreation;
JBTypes::timePointMs _timePointComeBack;

};
#endif /* CAMERA_H */
