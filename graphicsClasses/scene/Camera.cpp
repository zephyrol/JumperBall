/*
 * File: Camera.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#include "Camera.h"

Camera::Camera(const Map& map, float ratio)
    : _map(map),
      _chronometer(map.getBall()->getCreationChronometer()),
      _zFar(4.f * map.getLargestSize()),
      _movement(Camera::Movement::TurningAroundMap),
      _fovY(getFovY(ratio)),
      _offset(getOffset(ratio)),
      _pos(1.f, 0.f, 0.f),
      _center(0.f, 0.f, 0.f),
      _up(0.f, 1.f, 0.f),
      _timePointComeBack(0.f),
      _timePointGoAbove(0.f),
      _perspectiveMatrix(glm::perspective(getFovY(ratio), ratio, _offset.zNear, _zFar)) {}

void Camera::update(const Player::Status& status, bool goAbove) noexcept {
    if (status == Player::Status::InMenu) {
        _movement = Camera::Movement::TurningAroundMap;
    } else if (status == Player::Status::InGame) {
        _movement = Camera::Movement::FollowingBall;
    } else if (status == Player::Status::InTransition) {
        _movement = Camera::Movement::ApproachingBall;
    }

    // TODO : Replace by in game time
    const auto updatingTime = _chronometer->getTime();
    if (goAbove && _timePointComeBack >= _timePointGoAbove) {
        _timePointGoAbove = updatingTime;
    } else if (!goAbove && _timePointGoAbove >= _timePointComeBack) {
        _timePointComeBack = updatingTime;
    }
    if (_movement == Camera::Movement::TurningAroundMap) {
        turningAroundMapUpdate();
        return;
    }
    if (_movement == Camera::Movement::FollowingBall) {
        followingBallUpdate();
        return;
    }
    if (approachingBallUpdate()) {
        _movement = Camera::Movement::FollowingBall;
    }
}

const Camera::Movement& Camera::getMovement() noexcept {
    return _movement;
}

void Camera::followingBallUpdate() noexcept {
    const auto& ball = *_map.getBall();
    const auto lookingDirection = ball.lookTowardsAsVector();

    const auto stateBall = ball.state();

    const auto sideBall = ball.currentSideAsVector();
    const glm::vec3 toSkyVec3 = Utility::convertToOpenGLFormat(sideBall);

    const float timeSinceAction = ball.getTimeSecondsSinceAction();

    glm::mat4 matRotationCam(1.f);
    const glm::vec3 vecLookingDirection = Utility::convertToOpenGLFormat(lookingDirection);

    if (stateBall == Ball::State::Moving) {
        const JBTypes::vec3f vecNextLook = _map.getNextLook();

        const glm::vec3 axisNewLook = Utility::convertToOpenGLFormat(vecNextLook);

        const glm::vec3 axisRotation = glm::cross(vecLookingDirection, axisNewLook);
        const glm::vec3 eulerAngles =
            (timeSinceAction * JBTypes::pi2 / Ball::timeToGetNextBlock) * axisRotation;

        const glm::quat quaternion(eulerAngles);
        matRotationCam = glm::toMat4(quaternion);
    } else if (stateBall == Ball::State::TurningLeft || stateBall == Ball::State::TurningRight) {
        const glm::vec3& axisRotation = toSkyVec3;

        glm::vec3 eulerAngles = JBTypes::pi2 * axisRotation -
                                (timeSinceAction * JBTypes::pi2 / (Ball::timeToTurn)) * axisRotation;

        if (stateBall == Ball::State::TurningLeft) {
            eulerAngles = -eulerAngles;
        }

        const glm::quat quaternion(eulerAngles);
        matRotationCam = glm::toMat4(quaternion);
    }

    const auto cameraAboveWay = getAboveWay();
    const glm::vec3 axisRotation = glm::cross(vecLookingDirection, toSkyVec3);
    const glm::vec3 eulerAngles = cameraAboveWay * (-JBTypes::pi / 2.75f) * axisRotation;
    const glm::quat quaternion(eulerAngles);

    const glm::vec3 initCenterCam = _offset.targetDistance * vecLookingDirection;
    const glm::vec3 toCameraPosition = _offset.above * toSkyVec3 - behindCameraDistance * vecLookingDirection;

    const JBTypes::vec3f& position = ball.get3DPosition();
    const glm::mat4 matPosBall = glm::translate(Utility::convertToOpenGLFormat(position));
    matRotationCam = matRotationCam * glm::toMat4(quaternion);
    const glm::mat4 matPosBallRotationCam = matPosBall * matRotationCam;

    const glm::vec4 posVec = matPosBallRotationCam * glm::vec4(toCameraPosition, 1.f);
    _pos = posVec;

    const glm::vec4 centerVec = matPosBallRotationCam * glm::vec4(initCenterCam, 1.f);
    _center = centerVec;

    const glm::vec3 upVector = matRotationCam * glm::vec4(glm::cross(axisRotation, vecLookingDirection), 1.f);
    _up = upVector;
}

void Camera::turningAroundMapUpdate() noexcept {
    const auto xMax = static_cast<float>(_map.width());
    const auto yMax = static_cast<float>(_map.height());
    const auto zMax = static_cast<float>(_map.depth());

    const glm::vec3 center{(xMax - 1.f) / 2.f, (yMax - 1.f) / 2.f, (zMax - 1.f) / 2.f};

    const float distanceMax = std::max(std::max(xMax, yMax), zMax);

    const float cameraDistanceNear = (distanceMax / 2.f) / tanf(_offset.halfMinFov) * 1.1f;
    const float cameraDistanceFar = cameraDistanceNear * 1.3f;
    const float distanceX = cameraDistanceNear + (cameraDistanceFar - cameraDistanceNear) *
                                                     ((-cosf(_chronometer->getTime()) + 1.f) / 2.f);

    const float distanceZ = distanceX;
    const glm::mat4 translation = glm::translate(glm::vec3(distanceX, 0, distanceZ));

    const auto timeSinceCreation = _chronometer->getTime();
    const glm::mat4 rotation =
        glm::rotate(1.5f * timeSinceCreation,
                    glm::vec3(0.f, 1.f + sinf(timeSinceCreation / 5.f), 1.f + sinf(timeSinceCreation / 3.f)));
    const glm::mat4 transform = rotation * translation;

    const glm::vec4 positionTransform = transform * glm::vec4(0.f, 0.f, 0.f, 1.f);
    const glm::vec3 cameraPosition(positionTransform.x, positionTransform.y, positionTransform.z);

    const glm::vec4 up = rotation * glm::vec4(0.f, 1.f, 0.f, 1.f);

    _pos = cameraPosition;
    _center = center;
    _up = up;
}

bool Camera::approachingBallUpdate() noexcept {
    const JBTypes::vec3f& position = _map.getBall()->get3DPosition();

    constexpr float transitionDuration = 2.f;

    float t = _chronometer->getTime() / transitionDuration;

    constexpr float distanceXStarting = -10.f;
    constexpr float distanceYStarting = 5.f;
    constexpr float distanceZStarting = 10.f;

    bool animationIsFinished;
    if (t > 1.f) {
        t = 1.f;
        animationIsFinished = true;
    } else {
        animationIsFinished = false;
    }

    const auto& ball = *_map.getBall();
    const auto lookingDirection = ball.lookTowardsAsVector();
    const glm::vec3 vecLookingDirection = Utility::convertToOpenGLFormat(lookingDirection);

    const auto sideBall = ball.currentSideAsVector();
    const glm::vec3 toSkyVec3 = Utility::convertToOpenGLFormat(sideBall);

    const float tCos = cosf(t * JBTypes::pi2 + JBTypes::pi) + 1.f;

    const glm::vec3 directionVector = -glm::normalize(_center - _pos);
    const glm::mat4 upRotation = glm::rotate(tCos * 2.f * JBTypes::pi, directionVector);
    const glm::vec4 upVector = upRotation * glm::vec4(0.f, 1.f, 0.f, 1.f);

    const auto oneMinusTCos = 1.f - tCos;

    const glm::vec3 toCameraPosition = _offset.above * toSkyVec3 - behindCameraDistance * vecLookingDirection;
    const glm::vec3 initCenterCam = _offset.targetDistance * vecLookingDirection;

    const glm::mat4 matPosBall = glm::translate(Utility::convertToOpenGLFormat(position));
    const glm::vec4 posVec = matPosBall * glm::vec4(toCameraPosition, 1.f);
    const glm::vec4 centerVec = matPosBall * glm::vec4(initCenterCam, 1.f);

    _pos = {tCos * posVec.x + oneMinusTCos * (posVec.x + distanceXStarting),
            t * posVec.y + oneMinusTCos * (posVec.y + distanceYStarting),
            t * posVec.z + oneMinusTCos * (posVec.z + distanceZStarting)};
    _center = centerVec;
    _up = upVector;

    return animationIsFinished;
}

glm::mat4 Camera::genVPMatrixFromStar(const Star& star) {
    constexpr float offsetJumpingBall = 1.f;  // size of ball + jump height
    const float envSize = star.envSize();
    const float halfBoundingBoxSize = envSize / 2.f + offsetJumpingBall;

    // We use a close star position to get a better ZBuffer accuracy
    const JBTypes::vec3f& rotationCenter = star.rotationCenter();
    const glm::vec3 centerWorld = Utility::convertToOpenGLFormat(rotationCenter);

    const JBTypes::vec3f& position = star.position();
    const glm::vec3 closeStarPosition =
        centerWorld +
        glm::normalize((Utility::convertToOpenGLFormat(position)) - centerWorld) * halfBoundingBoxSize;

    constexpr auto starZNear = 0.1f;
    return glm::ortho(-halfBoundingBoxSize, halfBoundingBoxSize, -halfBoundingBoxSize, halfBoundingBoxSize,
                      starZNear, starZNear + 2.f * halfBoundingBoxSize) *
           glm::lookAt(closeStarPosition, centerWorld, glm::vec3(0.f, 1.f, 0.f));
}

const glm::vec3& Camera::pos() const noexcept {
    return _pos;
}

glm::mat4 Camera::viewProjection() const noexcept {
    const glm::mat4 viewMatrix = glm::lookAt(_pos, _center, _up);
    return _perspectiveMatrix * viewMatrix;
}
bool Camera::isMovingAbove() const {
    return getAboveWay() > 0.f;
}

float Camera::getAboveWay() const {
    float cameraAboveWay;
    constexpr float moveAboveDuration = 0.2f;

    const auto updatingTime = _chronometer->getTime();
    if (_timePointGoAbove > _timePointComeBack) {
        const float initialOffset =
            std::min((_timePointGoAbove - _timePointComeBack) / moveAboveDuration, 1.f);
        const float aboveCoeff = std::min((updatingTime - _timePointGoAbove) / moveAboveDuration, 1.f);
        cameraAboveWay = -(std::min(initialOffset - aboveCoeff, 1.f) - 1.f);
    } else {
        const float initialOffset =
            std::min((_timePointComeBack - _timePointGoAbove) / moveAboveDuration, 1.f);
        const float comingBackCoeff = std::min((updatingTime - _timePointComeBack) / moveAboveDuration, 1.f);
        cameraAboveWay = std::max(initialOffset - comingBackCoeff, 0.f);
    }
    cameraAboveWay = std::min(std::max(cameraAboveWay, 0.f), 1.f);
    return cameraAboveWay;
}

void Camera::setRatio(float ratio) {
    _offset = getOffset(ratio);
    _perspectiveMatrix = glm::perspective(getFovY(ratio), ratio, _offset.zNear, _zFar);
}

float Camera::getFovY(float ratio) noexcept {
    constexpr auto fovMin = 40.f;
    constexpr auto fovMax = 72.f;
    constexpr auto ratioMin = 0.5f;
    constexpr auto ratioMax = 2.f;
    const float croppedRatio = std::max(std::min(ratio, ratioMax), ratioMin);
    const float fovY = fovMax - (croppedRatio - ratioMin) / (ratioMax - ratioMin) * (fovMax - fovMin);
    return fovY * JBTypes::pi / 180.f;
}

Camera::Offset Camera::getOffset(float ratio) {
    constexpr auto initialAboveBallDistance = 1.8f;
    constexpr auto initialZNear = 0.2f;

    constexpr auto initialTargetDistance = 2.f;
    const auto gamma = atanf((initialTargetDistance + behindCameraDistance) / initialAboveBallDistance);

    const auto halfFovY = getFovY(ratio) / 2.f;
    const auto alpha = gamma - halfFovY;

    constexpr auto behindBallVisibility = 0.7f;
    constexpr auto maximalVisibilityDistance = behindCameraDistance - behindBallVisibility;

    const auto visibilityDistance = tanf(alpha) * initialAboveBallDistance;

    const auto halfMinFov = ratio > 1.f ? halfFovY : atanf(ratio * tanf(halfFovY));
    if (visibilityDistance < maximalVisibilityDistance) {
        std::cout << "initial " << initialAboveBallDistance<< std::endl;
        return {initialAboveBallDistance, initialTargetDistance, initialZNear, halfMinFov};
    }
    const auto aboveBallDistance = maximalVisibilityDistance / tanf(alpha);
    const auto groundDistance = tanf(gamma) * aboveBallDistance;
    const auto targetDistance = groundDistance - maximalVisibilityDistance;
        std::cout << "computed " << aboveBallDistance << std::endl;
    return {aboveBallDistance, targetDistance, initialZNear, halfMinFov};
}
