/*
 * File: Camera.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#ifndef CAMERA_H
#define CAMERA_H
#include <scene/Star.h>
#include <scene/Ball.h>
#include <scene/Map.h>
#include <scene/SceneElement.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Utility.h"

class Camera: public SceneElement {

public:

enum class Movement { TurningAroundMap, FollowingBall, ApproachingBall };

Camera(const Map& map, float ratio);

static constexpr float zNear = 0.1f;
static constexpr float zFar = 100.f;

const glm::vec3& pos() const noexcept;
glm::mat4 viewProjection() const noexcept;

SceneElement::GlobalState getGlobalState() const override;

void update() noexcept;
void turnAroundMap() noexcept;
void followBall() noexcept;
void approachBall() noexcept;
const Movement& getMovement() noexcept;
void setRatio(float ratio);

static glm::mat4 genVPMatrixFromStar(const Star& star);

private:

void turningAroundMapUpdate() noexcept;
void followingBallUpdate() noexcept;
bool approachingBallUpdate() noexcept;

static float computeRotationAngle(float fovy) noexcept;
static float computeFovy(float ratio) noexcept;
static float computeLocalOffset(float fovy) noexcept;

const Map& _map;
float _fovy;
float _localOffset;
Movement _movement;
glm::vec3 _pos;
glm::vec3 _center;
glm::vec3 _up;
bool _willComeBack;
bool _isComingBack;
float _cameraAboveWay;
float _timeSinceCreation;
JBTypes::timePointMs _timePointComeBack;
glm::mat4 _perspectiveMatrix;

static constexpr float distBehindBall = 0.5f;
static constexpr float distAbove = 1.1f;
static constexpr float distDirPoint = 2.f;
};
#endif /* CAMERA_H */
