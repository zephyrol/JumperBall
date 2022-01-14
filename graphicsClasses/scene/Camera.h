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
#include "player/Player.h"
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

void update(
    const JBTypes::timePointMs& updatingTime,
    const Player::Status& status,
    bool goAbove
) noexcept;
const Movement& getMovement() noexcept;
void setRatio(float ratio);

static glm::mat4 genVPMatrixFromStar(const Star& star);

private:

void turningAroundMapUpdate() noexcept;
void followingBallUpdate() noexcept;
bool approachingBallUpdate() noexcept;

static float computeFovy(float ratio) noexcept;
static float computeLocalOffset(float fovy) noexcept;

const Map& _map;
const JBTypes::timePointMs _creationTime;
JBTypes::timePointMs _updatingTime;
float _fovy;
float _localOffset;
Movement _movement;
glm::vec3 _pos;
glm::vec3 _center;
glm::vec3 _up;
float _timeSinceCreation;
JBTypes::timePointMs _timePointComeBack;
JBTypes::timePointMs _timePointGoAbove;
glm::mat4 _perspectiveMatrix;

static constexpr float distBehindBall = 0.5f;
static constexpr float distAbove = 1.1f;
static constexpr float distDirPoint = 2.f;

};
#endif /* CAMERA_H */
