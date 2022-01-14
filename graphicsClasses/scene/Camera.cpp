/*
 * File: Camera.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 13:12
 */

#include "Camera.h"

Camera::Camera(const Map& map, float ratio):
    _map(map),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _updatingTime(_creationTime),
    _fovy(computeFovy(ratio)),
    _localOffset(computeLocalOffset(_fovy)),
    _movement(Camera::Movement::TurningAroundMap),
    _pos(1.f, 0.f, 0.f),
    _center(0.f, 0.f, 0.f),
    _up(0.f, 1.f, 0.f),
    _timeSinceCreation(0),
    _timePointComeBack(_creationTime),
    _timePointGoAbove(_creationTime),
    _perspectiveMatrix(glm::perspective(_fovy, ratio, zNear, zFar))
{
}

void Camera::update(
    const JBTypes::timePointMs& updatingTime,
    const Player::Status& status,
    bool goAbove
) noexcept{

    if (status == Player::Status::INMENU) {
        _movement = Camera::Movement::TurningAroundMap;
    }
    else if (status == Player::Status::INGAME) {
        _movement = Camera::Movement::FollowingBall;
    }
    else if (status == Player::Status::INTRANSITION) {
        _movement = Camera::Movement::ApproachingBall;
    }

    _updatingTime = updatingTime;
    if (goAbove && _timePointComeBack >= _timePointGoAbove) {
        _timePointGoAbove = _updatingTime;
    } else if (!goAbove && _timePointGoAbove >= _timePointComeBack) {
        _timePointComeBack = _updatingTime;
    }
    _timeSinceCreation = JBTypesMethods::getFloatFromDurationMS(_updatingTime - _creationTime);
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
const Camera::Movement& Camera::getMovement() noexcept{
    return _movement;
}

void Camera::followingBallUpdate() noexcept{

    const auto& ball = *_map.getBall();
    const auto lookingDirection = ball.lookTowardsAsVector();

    const auto stateBall = ball.state();

    constexpr float durationMoveAbove = 0.2f;

    const auto sideBall = ball.currentSideAsVector();
    const glm::vec3 toSkyVec3 = Utility::convertToOpenGLFormat(sideBall);
    const glm::vec4 toSky(toSkyVec3, 1.f);

    const float timeSinceAction = ball.getTimeSecondsSinceAction();

    glm::mat4 matRotationCam(1.f);
    const glm::vec3 vecLookingDirection = Utility::convertToOpenGLFormat(lookingDirection);

    if (stateBall == Ball::State::Moving) {
        const JBTypes::vec3f vecNextLook = _map.getNextLook();

        const glm::vec3 axisNewLook = Utility::convertToOpenGLFormat(vecNextLook);

        const glm::vec3 axisRotation = glm::cross(vecLookingDirection, axisNewLook);
        const glm::vec3 eulerAngles = (timeSinceAction *
                                       (static_cast <float>(M_PI_2)) / Ball::timeToGetNextBlock) *
                                      axisRotation;

        const glm::quat quaternion(eulerAngles);
        matRotationCam = glm::toMat4(quaternion);
    } else if ( stateBall == Ball::State::TurningLeft || stateBall == Ball::State::TurningRight ) {
        const glm::vec3& axisRotation = toSkyVec3;

        glm::vec3 eulerAngles = static_cast <float>(M_PI_2) * axisRotation -
            (timeSinceAction * static_cast <float>(M_PI_2) / (Ball::timeToTurn)) * axisRotation;

        if (stateBall == Ball::State::TurningLeft) {
            eulerAngles = -eulerAngles;
        }

        const glm::quat quaternion(eulerAngles);
        matRotationCam = glm::toMat4(quaternion);

    }

    float cameraAboveWay;
    if (_timePointGoAbove > _timePointComeBack) {
        const float initialOffset = std::min(JBTypesMethods::getFloatFromDurationMS(
            _timePointGoAbove - _timePointComeBack
        ) / durationMoveAbove, 1.f);
        const float aboveCoeff = std::min(JBTypesMethods::getFloatFromDurationMS(
            _updatingTime - _timePointGoAbove
        ) / durationMoveAbove, 1.f);
        cameraAboveWay = -(std::min(initialOffset - aboveCoeff, 1.f) - 1.f);
    } else {
        const float initialOffset = std::min(JBTypesMethods::getFloatFromDurationMS(
            _timePointComeBack - _timePointGoAbove
        ) / durationMoveAbove, 1.f);
        const float comingBackCoeff = std::min(JBTypesMethods::getFloatFromDurationMS(
            _updatingTime - _timePointComeBack
        ) / durationMoveAbove, 1.f);
        cameraAboveWay = std::max(initialOffset - comingBackCoeff, 0.f);
    }

    const glm::vec3 axisRotation = glm::cross(vecLookingDirection, toSkyVec3);
    const glm::vec3 eulerAngles = cameraAboveWay * static_cast <float>(-M_PI / 2.75) * axisRotation;
    const glm::quat quaternion(eulerAngles);

    const glm::vec3 toInitialCameraPosition = distAbove * toSkyVec3 - distBehindBall * vecLookingDirection;
    const glm::vec3 initCenterCam = distDirPoint * vecLookingDirection;
    const glm::vec3 offsetDirection = glm::normalize(initCenterCam - toInitialCameraPosition);
    const glm::vec3 initPosCam = toInitialCameraPosition + _localOffset * offsetDirection;


    const JBTypes::vec3f& position = ball.get3DPosition();
    const glm::mat4 matPosBall = glm::translate(Utility::convertToOpenGLFormat(position));
    matRotationCam = matRotationCam * glm::toMat4(quaternion);
    const glm::mat4 matPosBallRotationCam = matPosBall * matRotationCam;

    const glm::vec4 posVec =  matPosBallRotationCam * glm::vec4(initPosCam, 1.f);
    _pos = posVec;

    const glm::vec4 centerVec = matPosBallRotationCam * glm::vec4(initCenterCam, 1.f);
    _center = centerVec;

    const glm::vec3 upVector = matRotationCam * glm::vec4(glm::cross(axisRotation, vecLookingDirection), 1.f);
    _up = upVector;

}

void Camera::turningAroundMapUpdate() noexcept{

    const auto xMax = static_cast<float>(_map.width());
    const auto yMax = static_cast<float>(_map.height());
    const auto zMax = static_cast<float>(_map.depth());

    const glm::vec3 center { (xMax - 1.f) / 2.f, (yMax - 1.f) / 2.f, (zMax - 1.f) / 2.f };

    float distanceMax = xMax;
    if (distanceMax < yMax) distanceMax = yMax;
    if (distanceMax < zMax) distanceMax = zMax;

    const float cameraDistanceNear = distanceMax * 1.f;
    const float cameraDistanceFar = distanceMax * 1.3f;
    const float diffF = _timeSinceCreation;
    const float distanceX = cameraDistanceNear +
                            (cameraDistanceFar - cameraDistanceNear) *
                            ((-cosf(diffF) + 1.f) / 2.f);

    const float distanceZ = distanceX;

    const glm::mat4 translation = glm::translate(glm::vec3(distanceX, 0, distanceZ));

    const glm::mat4 rotation = glm::rotate(
        1.5f * diffF,
        glm::vec3(0.f, 1.f + sinf(diffF / 5.f), 1.f + sinf(diffF / 3.f))
        );
    const glm::mat4 transform = rotation * translation;


    const glm::vec4 positionTransform = transform * glm::vec4(0.f, 0.f, 0.f, 1.f);
    const glm::vec3 positionCamera(positionTransform.x, positionTransform.y, positionTransform.z);

    const glm::vec4 up = rotation * glm::vec4(0.f, 1.f, 0.f, 1.f);

    _pos = positionCamera;
    _center = center;
    _up = up;

}

bool Camera::approachingBallUpdate() noexcept{
    bool animationIsFinished;

    const JBTypes::vec3f& position = _map.getBall()->get3DPosition();

    const float diffF = _timeSinceCreation;
    constexpr float transitionDuration = 2.f;
    float t = diffF / transitionDuration;

    constexpr float distanceXStarting = 10.f;
    constexpr float distanceYStarting = 5.f;
    constexpr float distanceZStarting = 10.f;

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
    const glm::vec4 toSky(toSkyVec3, 1.f);

    const float tCos = cosf(t * static_cast <float>(M_PI_2) + static_cast <float>(M_PI)) + 1.f;

    const glm::vec3 directionVector = glm::normalize(_center - _pos);
    const glm::mat4 upRotation = glm::rotate(tCos * 2.f * static_cast <float>(M_PI), directionVector);
    const glm::vec4 upVector = upRotation * glm::vec4(0.f, 1.f, 0.f, 1.f);

    const auto oneMinusTCos = 1.f - tCos;

    const glm::vec3 toInitialCameraPosition = distAbove * toSkyVec3 - distBehindBall * vecLookingDirection;
    const glm::vec3 initCenterCam = distDirPoint * vecLookingDirection;
    const glm::vec3 offsetDirection = glm::normalize(initCenterCam - toInitialCameraPosition);
    const glm::vec3 initPosCam = toInitialCameraPosition + _localOffset * offsetDirection;

    const glm::mat4 matPosBall = glm::translate(Utility::convertToOpenGLFormat(position));
    const glm::vec4 posVec =  matPosBall * glm::vec4(initPosCam, 1.f);
    const glm::vec4 centerVec = matPosBall * glm::vec4(initCenterCam, 1.f);

    _pos = {
        tCos * posVec.x + oneMinusTCos * (posVec.x + distanceXStarting),
        t * posVec.y + oneMinusTCos * (posVec.y + distanceYStarting),
        t * posVec.z + oneMinusTCos * (posVec.z + distanceZStarting)
    };
    _center = centerVec;
    _up = upVector;

    return animationIsFinished;
}

glm::mat4 Camera::genVPMatrixFromStar (const Star& star) {

    constexpr float offsetJumpingBall = 1.f; // size of ball + jump height
    const float envSize = star.envSize();
    const float halfBoundingBoxSize = envSize / 2.f + offsetJumpingBall;

    // We use a close star position to get a better ZBuffer accuracy
    const JBTypes::vec3f& rotationCenter = star.rotationCenter();
    const glm::vec3 centerWorld = Utility::convertToOpenGLFormat(rotationCenter);

    const JBTypes::vec3f& position = star.position();
    const glm::vec3 closeStarPosition = centerWorld + glm::normalize(
        (Utility::convertToOpenGLFormat(position)) - centerWorld
        )  * halfBoundingBoxSize;

    return glm::ortho(
        -halfBoundingBoxSize,
        halfBoundingBoxSize,
        -halfBoundingBoxSize,
        halfBoundingBoxSize,
        zNear, zNear + 2.f * halfBoundingBoxSize
        ) * glm::lookAt(closeStarPosition, centerWorld, glm::vec3(0.f, 1.f, 0.f));

}

const glm::vec3& Camera::pos() const noexcept{
    return _pos;
}

glm::mat4 Camera::viewProjection() const noexcept{
    const glm::mat4 viewMatrix = glm::lookAt(_pos, _center, _up);
    return _perspectiveMatrix * viewMatrix;
}

SceneElement::GlobalState Camera::getGlobalState() const {
    return SceneElement::GlobalState::United;
}

void Camera::setRatio(float ratio) {
    _fovy = computeFovy(ratio);
    _localOffset = computeLocalOffset(_fovy);
    _perspectiveMatrix = glm::perspective(_fovy, ratio, zNear, zFar);
}

float Camera::computeFovy(float ratio) noexcept {
    constexpr float defaultFovy = 1.22173f;// 70 degrees;
    return ratio > 1.f
        ? defaultFovy
        : 2.f * atanf((1.f / ratio) * tanf(defaultFovy/ 2.f));
}

float Camera::computeLocalOffset(float fovy) noexcept {
    const float beta = atanf((distBehindBall + distDirPoint) / distAbove);
    const float halfFovy = fovy / 2.f;
    if (beta < halfFovy) {
        const float alpha = static_cast<float>(M_PI) - halfFovy;
        const float gamma = static_cast<float>(M_PI) - alpha - beta;

        // Sines law
        return distAbove * sinf(gamma) / sinf(alpha);
    }

    const float alpha = static_cast<float>(M_PI) - beta;
    const float gamma = static_cast<float>(M_PI) - halfFovy - alpha;
    // Sines law
    return -distAbove * sinf(gamma) / sinf(halfFovy); // minus because the camera is behind
}

